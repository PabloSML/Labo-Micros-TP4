/***************************************************************************//**
  @file     uartrxfifo.h
  @brief    Uart RX Fifo Header File
  @author   Grupo 4
 ******************************************************************************/

#ifndef _UARTRX_FIFO_H_
#define _UARTRX_FIFO_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdlib.h>
#include "uartrxfifo_config.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



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
 * @brief Initialize queue
 */
void uartrx_fifo_init(void);

/**
 * @brief Insert data in circular FIFO
 * @param data Data intended for insert
 * @return Queue state after insert
 */
uint8_t uartrx_fifo_queue(uartrxfifoEl_t data);

/**
 * @brief Read data from circular FIFO
 * @return Data read from queue front
 */
uartrxfifoEl_t uartrx_fifo_dequeue(void);

/**
 * @brief Get status from circular FIFO
 * @return Number of elements in queue. Zero if empty
 */
uint8_t uartrx_fifo_getStatus(void);

/**
 * @brief Reset FIFO to empty status
 */
void uartrx_fifo_reset(void);

/*******************************************************************************
 ******************************************************************************/

#endif // _UARTRX_FIFO_H_
