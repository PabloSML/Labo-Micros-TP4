/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Grupo 4
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "board.h"
#include "uart.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/



/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
static uint8_t msgLen = 10;
static uint8_t testMsgTx[10] = {1,2,3,4,5,6,7,8,9,10};
static uint8_t testMsgRx[10];
static uint8_t* rxpointer = testMsgRx;
static uint8_t recievedLen;
static bool stop = false;
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{

  // Inits for FRDM
  boardInit();
  uart_cfg_t u3Config = {.baudrate=9600, .bitcant=8, .parity=NO_PARITY, .sbns=1};
  uartInit(UART3_ID, u3Config);

}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{

  if(uartIsTxMsgComplete(UART3_ID) && !(stop))
  {
    uartWriteMsg(UART3_ID, testMsgTx, msgLen);
    stop = true;
  }

  if(uartIsRxMsg(UART3_ID))
  {
    recievedLen = uartGetRxMsgLength(UART3_ID);
    if(recievedLen == 10)
    {
      uartReadMsg(UART3_ID, rxpointer, recievedLen);
    }
  }
  
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



/*******************************************************************************
 ******************************************************************************/
