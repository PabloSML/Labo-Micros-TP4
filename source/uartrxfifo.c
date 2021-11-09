/***************************************************************************//**
  @file     uartrxfifo.c
  @brief    Uart RX Fifo Source File.
  @author   Grupo 4
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "uartrxfifo.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef uartrxfifoEl_t uartrxfifoPointer_t;

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

static uartrxfifoEl_t queue[UARTRX_FIFO_SIZE];     // Circular buffer
static uartrxfifoPointer_t* frontPointer;   // Pointer to front of queue
static uartrxfifoPointer_t* rearPointer;    // Pointer to rear of queue
static uint8_t news;                  // Status flag and pending data counter

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void uartrx_fifo_init(void)
{
  frontPointer = queue;     // Set queue pointers to start of buffer
  rearPointer = queue;
  news = 0;                 // Clear status flag
}


uint8_t uartrx_fifo_queue(uartrxfifoEl_t data)
{
  if (news == UARTRX_FIFO_SIZE)    // Test for Queue overflow
	{
		news = UARTRX_FIFO_OVERFLOW;   // Inform queue has overflowed
		return news;		
	}	

	*(rearPointer++) = data;	// Push data
	news++;				            // Update "news" counter

	if (rearPointer == (queue + UARTRX_FIFO_SIZE))	// If rear has exceded reset pointer
		rearPointer = queue;

	return news;			        // Inform Queue state
}


uartrxfifoEl_t uartrx_fifo_dequeue(void)
{
  uartrxfifoEl_t data;

	data = *(frontPointer++); // Pull data
	news--;                   // Update "news" counter

	if (frontPointer == queue + UARTRX_FIFO_SIZE)	// Check for Queue boundaries
		frontPointer = queue;		// If front has exceded reset pointer

	return data;              // Return retrieved data
}

uint8_t uartrx_fifo_getStatus(void)
{
  return news;
}

void uartrx_fifo_reset(void)
{
  uartrx_fifo_init();
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/******************************************************************************/
