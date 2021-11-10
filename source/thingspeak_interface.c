/***************************************************************************//**
  @file     thingspeak_interface.c
  @brief    Thingspeak Interface Source File
  @author   Grupo 4
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "uart.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// Gateway definitions
#define GW_PREFIX               0xAA, 0x55, 0xC3, 0x3C
#define GW_PREFIX_OFFSET        4
#define GW_LEN_OFFSET           1

#define TS_SENDBUFF_LEN         32

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/



/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/



/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static bool yaInit = false;

static uint8_t sendBuffer[TS_SENDBUFF_LEN] = {GW_PREFIX}; //Init first values with prefix

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void thingspeak_init(void)
{
  if(!yaInit)
  {
    uart_cfg_t u0Config = {.baudrate=1200, .bitcant=8, .parity=NO_PARITY, .sbns=1};
    uartInit(UART0_ID, u0Config);

    yaInit = true;
  }
}

bool thingspeak_tx(const uint8_t* msg, uint8_t msgLen)
{
  bool success = true;

  sendBuffer[GW_PREFIX_OFFSET] = msgLen;

  uint8_t i;
  for(i = 0; i < msgLen; i++)
  {
    sendBuffer[i + GW_PREFIX_OFFSET + GW_LEN_OFFSET] = msg[i];
  }

  if(uartIsTxMsgComplete(UART0_ID))
    uartWriteMsg(UART0_ID, sendBuffer, (GW_PREFIX_OFFSET+GW_LEN_OFFSET+msgLen));
  else
    success = false;

  return success;
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/******************************************************************************/
