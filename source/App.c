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
#include "gpio_pdrv.h"
#include "logic_module.h"
#include <os.h>

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
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (OS_TCB* startTCB_p)
{

  // Inits for DJ_BOARD
  boardInit();
  ledInit();
  buttonInit(startTCB_p);
  encoderInit(startTCB_p);
  sevenSegInit();
  magneticReaderInit(startTCB_p);
  logic_module_init(startTCB_p);

}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void* msg)
{
  run_logic_module(msg);

	// if(!(stop))
	// {
	// 	thingspeak_tx(testMsgTx, 7);
	// 	stop = true;
	// }
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



/*******************************************************************************
 ******************************************************************************/
