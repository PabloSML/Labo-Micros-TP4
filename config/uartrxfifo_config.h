/***************************************************************************//**
  @file     uartrxfifo_config.h
  @brief    UART RX Fifo Config File
  @author   Grupo 4
 ******************************************************************************/

#ifndef _UARTRX_FIFO_CONFIG_H_
#define _UARTRX_FIFO_CONFIG_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define UARTRX_FIFO_OVERFLOW     0xFF  // Overflow character
#define UARTRX_FIFO_SIZE         32    // Fifo size in number of elements. Must be less than FIFO_OVERFLOW

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef uint8_t uartrxfifoEl_t;         // FIFO Element Type

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/


/*******************************************************************************
 ******************************************************************************/

#endif // _UARTRX_FIFO_CONFIG_H_
