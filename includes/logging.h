/***************************************************************************//**
  @file     logging.h
  @brief    Logging Thread Header File
  @author   Grupo 4
 ******************************************************************************/

#ifndef _LOGGING_H_
#define _LOGGING_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>


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
 * @brief Initialize logging thread
 */
void logging_init(void);

/**
 * @brief notifies corresponding modules about floor ingress 
 * @param floor floor number
 */ 
void notify_ingress(uint8_t floor);

/*******************************************************************************
 ******************************************************************************/

#endif // _LOGGING_H_
