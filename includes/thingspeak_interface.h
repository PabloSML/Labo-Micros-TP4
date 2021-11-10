/***************************************************************************//**
  @file     thingspeak_interface.h
  @brief    Thingspeak Interface Header File
  @author   Grupo 4
 ******************************************************************************/

#ifndef _TS_INT_H_
#define _TS_INT_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define GW_COMMAND_SEND         0x01
#define GW_COMMAND_KEEPALIVE    0x02
#define GW_ANSWER_SENDDATAOK    0x81
#define GW_ANSWER_SENDDATAFAIL  0xC1
#define GW_ANSWER_KEEPALIVEOK   0x82

#define GW_DATABUFFER_LEN       7

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initialize Thingspeak interface
 */
void thingspeak_init(void);

/**
 * @brief Transmit to Thingspeak
 * @param msg Data intended for transmission
 * @param msgLen Number of bytes to transmit
 * @return True if transmission declaration succesful, false otherwise
 */
bool thingspeak_tx(const uint8_t* msg, uint8_t msgLen);

// /**
//  * @brief Recieve from Thingspeak
//  * @param msg Buffer for data reception
//  * @param msgLen Number of bytes to read
//  * @return True if transmission declaration succesful, false otherwise
//  */
// bool thingspeak_rx(uint8_t* msg, uint8_t msgLen);

/*******************************************************************************
 ******************************************************************************/

#endif // _TS_INT_H_
