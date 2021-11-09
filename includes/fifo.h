/***************************************************************************//**
  @file     fifo.h
  @brief    Fifo Header File
  @author   Grupo 4
 ******************************************************************************/

#ifndef _FIFO_H_
#define _FIFO_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdlib.h>
#include "fifo_config.h"

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
void fifo_init(void);

/**
 * @brief Insert data in circular FIFO
 * @param data Data intended for insert
 * @return Queue state after insert
 */
uint8_t fifo_queue(fifoEl_t data);

/**
 * @brief Read data from circular FIFO
 * @return Data read from queue front
 */
fifoEl_t fifo_dequeue(void);

/**
 * @brief Get status from circular FIFO
 * @return Number of elements in queue. Zero if empty
 */
uint8_t fifo_getStatus(void);

/**
 * @brief Reset FIFO to empty status
 */
void fifo_reset(void);

/*******************************************************************************
 ******************************************************************************/

#endif // _FIFO_H_
