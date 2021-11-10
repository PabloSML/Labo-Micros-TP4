/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Grupo 4
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "board.h"
#include "button_drv.h"
#include "led_drv.h"
#include "7seg_drv.h"
#include "encoder_drv.h"
#include "magnetic_reader_drv.h"
#include "uart.h"
#include "gpio_pdrv.h"
#include "logic_module.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/



/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static bool stop = false;
static const uint8_t testMsgTx = 0x55;
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{

  // Inits for DJ_BOARD
  boardInit();
  ledInit();
  buttonInit();
  encoderInit();
  sevenSegInit();
  uart_cfg_t u0Config = {.baudrate=1200, .bitcant=8, .parity=NO_PARITY, .sbns=1};
  uartInit(UART0_ID, u0Config);
  magneticReaderInit();
  logic_module_init();

}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
  // run_logic_module();

	if(uartIsTxMsgComplete(UART0_ID) && !(stop))
	{
		uartWriteMsg(UART0_ID, &testMsgTx, 1);
		stop = true;
	}
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



/*******************************************************************************
 ******************************************************************************/
