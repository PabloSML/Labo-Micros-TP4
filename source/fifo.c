/***************************************************************************//**
  @file     fifo.c
  @brief    Fifo Source File.
  @author   Grupo 4
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "fifo.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef fifoEl_t fifoPointer_t;

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

static fifoEl_t queue[FIFO_SIZE];     // Circular buffer
static fifoPointer_t* frontPointer;   // Pointer to front of queue
static fifoPointer_t* rearPointer;    // Pointer to rear of queue
static uint8_t news;                  // Status flag and pending data counter

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void fifo_init(void)
{
  frontPointer = queue;     // Set queue pointers to start of buffer
  rearPointer = queue;
  news = 0;                 // Clear status flag
}


uint8_t fifo_queue(fifoEl_t data)
{
  if (news == FIFO_SIZE)    // Test for Queue overflow
	{
		news = FIFO_OVERFLOW;   // Inform queue has overflowed
		return news;		
	}	

	*(rearPointer++) = data;	// Push data
	news++;				            // Update "news" counter

	if (rearPointer == (queue + FIFO_SIZE))	// If rear has exceded reset pointer
		rearPointer = queue;

	return news;			        // Inform Queue state
}


fifoEl_t fifo_dequeue(void)
{
  fifoEl_t data;

	data = *(frontPointer++); // Pull data
	news--;                   // Update "news" counter

	if (frontPointer == queue + FIFO_SIZE)	// Check for Queue boundaries
		frontPointer = queue;		// If front has exceded reset pointer

	return data;              // Return retrieved data
}

uint8_t fifo_getStatus(void)
{
  return news;
}

void fifo_reset(void)
{
  fifo_init();
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/******************************************************************************/
