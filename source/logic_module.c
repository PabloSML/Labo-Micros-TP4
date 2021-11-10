/***************************************************************************//**
  @file     logic_module.c
  @brief    Logic Module Source File
  @author   Grupo 4
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "logic_module.h"
#include "thingspeak_interface.h"
#include "decoder.h"
#include "led_drv.h"
#include "gpio_pdrv.h"
#include "timer_drv.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define UPLOADED_CRED_CANT	7
#define VALID_IDS 			{11111111UL, 51870110UL, 57586984UL, 57046507UL, 51870114UL, 52404904UL, 58663114UL}
#define VALID_PINS			{1111UL,1111UL,1111UL,1111UL,1111UL,1111UL,1111UL}
#define VALID_FLOORS		{FLOOR_1, FLOOR_1, FLOOR_1, FLOOR_2, FLOOR_2, FLOOR_3, FLOOR_3}

#define ID_LEN			8

#define FLOOR1_LSB		1
#define FLOOR2_LSB		3
#define FLOOR3_LSB		5

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum {
	FLOOR_1,
	FLOOR_2,
	FLOOR_3
} building_floors_t;

// Credential type
typedef struct{
	uint64_t id;
	uint64_t pin;
	building_floors_t floor;
} credential_t;

// // Credential database type
//  typedef struct {
// 	uint64_t valid_IDs[7];
// 	uint64_t valid_PINs[7];
//  } credential_dbase_t;
 
// typedef struct{
// 	uint8_t floor1Occup;
// 	uint8_t floor2Occup;
// 	uint8_t floor3Occup;
// } building_occupancy_t;  Maybe not needed

/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

/**
 * @brief Checks if input ID is a valid one registered in the data base
 * @return bool
 */
static bool check_ID();

/**
 * @brief Checks if input PIN is a valid one registered in the data base
 * @return bool
 */
static bool check_PIN();

/**
 * @brief converts ID from the magnetic reader in array format to digit format 
 * @return void 
 */ 
static void convert_ID();

/**
 * @brief Initialize the data base with IDs and PINs 
 * @return void
 */
static void upload_valid_credentials();

/**
 * @brief notifies corresponding modules about floor ingress 
 * @param floor floor number
 */ 
static void notify_ingress(building_floors_t floor);

/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static uint8_t ID_array[8];
// static uint8_t PIN_array[5] = {-1};
static uint8_t posc_ID = 0;

static uint64_t ID=0;
static uint64_t PIN=0;
// static credential_dbase_t valid_credentials;
static credential_t currentCred;
static credential_t valid_credentials[UPLOADED_CRED_CANT];

static DecoderEvent_t decoderEv = DECODER_noev;

static MagReaderEvent_t magreaderEv = MAGREADER_noev;
static uint8_t panLen = 0;

static bool waiting_for_PIN = false;
static DecoderType_t estado = DECODER_intensity;

static uint8_t occupancy[GW_DATABUFFER_LEN] = {GW_COMMAND_SEND}; // F1_LSB = 1, F2_LSB = 3, F3_LSB = 5 (Little Endian)
// static building_occupancy_t occupancy = {.floor1Occup = 0, .floor2Occup = 0, .floor3Occup = 0}; Maybe not needed

static tim_id_t tsTestTimerId;
static ttick_t tsTestTicks = 100000;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


bool logic_module_init(void)
{
  static bool yaInit = false;
    
  if (!yaInit) // init peripheral
  {
    upload_valid_credentials();
	ledInit();
	thingspeak_init();
	decoder(DECODER_intensity);
	tsTestTimerId = timerGetId();
	timerStart(tsTestTimerId, tsTestTicks, TIM_MODE_SINGLESHOT, 0);
	yaInit = true;
  }

  return yaInit;
}

void run_logic_module(void)
{

	if(decoder_hasEvent())
	{
		decoderEv = decoder_getEvent(); //fetch event
		switch(decoderEv)
		{
	    case DECODER_inputnum: //From encoder
			switch(estado){
				case DECODER_intensity:
					estado = DECODER_id; 
					ledOn(LED_3);  //prendo los LEDs como indicador 
					ledOff(LED_2);
					ledOff(LED_1);
					decoder(estado); //cambio el estado del decoder 
					break; 
				case DECODER_id:
					currentCred.id = decoder_getNumber(); 
					if (check_ID()) 
					{
						estado = DECODER_pin;
						ledOn(LED_3);
						ledOn(LED_2);
						ledOff(LED_1);
						decoder(estado);
					}
					else{
						estado = DECODER_invalid; //detecto un error en el ID
						ledBlink(LED_1, 500U); //parpadean todos los LEDs por 5 segundos 
						ledBlink(LED_2, 500U);
						ledBlink(LED_3, 500U);
						decoder(estado);
					}
					break;
				case DECODER_pin:
					currentCred.pin = decoder_getNumber();
					if(check_PIN())
					{
						estado = DECODER_open;
						if(timerExpired(tsTestTimerId))
						{
							notify_ingress(valid_credentials[posc_ID].floor);  // Si ID y PIN validos, se notifica a thingspeak
							timerStart(tsTestTimerId, tsTestTicks, TIM_MODE_SINGLESHOT, 0);
						}
						ledOn(LED_3);
						ledOn(LED_2);
						ledOn(LED_1);
						decoder(estado);
					}
					else{
						estado = DECODER_invalid;   //detecto un error en el PIN
						ledBlink(LED_1, 500U);  //parpadean todos los LEDs por 5 segundos 
						ledBlink(LED_2, 500U);
						ledBlink(LED_3, 500U);
						decoder(estado);
					}
					break;
				default:
					break;
			}
			break;
		
		case DECODER_restart:
			ID = 0;
			PIN = 0;
			if (estado == DECODER_invalid)
			{
				estado = DECODER_id;
				ledOn(LED_3);
				ledOff(LED_2);
				ledOff(LED_1);
			}
			else
			{
				estado = DECODER_intensity;
				ledOff(LED_3);
				ledOff(LED_2);
				ledOff(LED_1);
			}
			decoder(estado); //Lo mando setear la intensidad o id si fue inválido
	        break;

		case DECODER_inputerror:
			decoder(estado);	//Si le falto un numero en id, que siga en ID, lo mismo para PIN
			break;

	    default:
	        
	        break;
		}
	}	

	else if(magreader_hasEvent()&& (!waiting_for_PIN))
	{
		magreaderEv = magreader_getEvent();  //fetch event
		
		switch(magreaderEv)
		{

			case MAGREADER_cardUpload: //From magnetic card
				// panLen = getPANlen();
	    		// if(panLen >= ID_LEN)
				{
					uint8_t * pan = magreader_getRawData();
					for(uint8_t i = 0; i < ID_LEN; i++){
						ID_array[i] = (pan[i] % 10);
					}
	    			convert_ID();
					if(check_ID())
					{
						estado = DECODER_pin;
						ledOn(LED_3);
						ledOn(LED_2);
						ledOff(LED_1);
						decoder(estado);
					}
				}
				break;

			case MAGREADER_carderror: 

				break;

			case MAGREADER_cardsliding: 

				break;
		
			default:
				
				break;

		}
	}
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
static bool check_ID()
{
	uint8_t posc = 0;
	bool valid_ID = false;
	// uint8_t n = (uint8_t)(sizeof(valid_credentials.valid_IDs)/sizeof(valid_credentials.valid_IDs[0])); //detecto la cantidad de elementos 

	while((posc < UPLOADED_CRED_CANT) && (valid_ID == false))
	{
		if (valid_credentials[posc].id == currentCred.id)
			valid_ID = true;
		else
			posc++;
	}
	posc_ID = posc;  //guardo la posición del ID válido

	return valid_ID;
}

void upload_valid_credentials(void)
{
	uint64_t validIDs[UPLOADED_CRED_CANT] = VALID_IDS;
	uint64_t validPINs[UPLOADED_CRED_CANT] = VALID_PINS;
	building_floors_t validFloors[UPLOADED_CRED_CANT] = VALID_FLOORS;

	for(uint8_t i = 0; i < UPLOADED_CRED_CANT; i++)
	{
		valid_credentials[i].id = validIDs[i];
		valid_credentials[i].pin = validPINs[i];
		valid_credentials[i].floor = validFloors[i];
	}
}


static bool check_PIN(void)
{
	bool valid_PIN = false;

	if(valid_credentials[posc_ID].pin == currentCred.pin)
		valid_PIN = true;

	return valid_PIN;
}

static void convert_ID(void)
{
	int i = 0;
	// uint8_t n = (uint8_t)(sizeof(ID_array)/sizeof(ID_array[0])); //detecto la cantidad de elementos 
	currentCred.id = 0;

	for (i = 0; i < ID_LEN; i++)
		currentCred.id = currentCred.id*10 + ID_array[i];  //ID en formato "digitos"
}

static void notify_ingress(building_floors_t floor)
{

	occupancy[FLOOR1_LSB + 2*floor]++;
	thingspeak_tx(occupancy, GW_DATABUFFER_LEN);

}



/******************************************************************************/
