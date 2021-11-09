/***************************************************************************//**
  @file     7seg_config.h
  @brief    Seven Segment Driver Config File
  @author   Grupo 4
 ******************************************************************************/

#ifndef _7SEG_CONFIG_H_
#define _7SEG_CONFIG_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "board.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// Period for ISR

#define DISPLAY_ISR_PERIOD            1U  // 0.2 ms
#define NEXT_DISPLAY_REFRESH_TICKS    6U


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/



/*******************************************************************************
 ******************************************************************************/

#endif // _7SEG_CONFIG_H_
