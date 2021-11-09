/***************************************************************************//**
  @file     uarttxfifo.c
  @brief    Uart TX Fifo Source File.
  @author   Grupo 4
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "uarttxfifo.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef uarttxfifoEl_t uarttxfifoPointer_t;

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

static uarttxfifoEl_t queue[UARTTX_FIFO_SIZE];     // Circular buffer
static uarttxfifoPointer_t* frontPointer;   // Pointer to front of queue
static uarttxfifoPointer_t* rearPointer;    // Pointer to rear of queue
static uint8_t news;                  // Status flag and pending data counter

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void uarttx_fifo_init(void)
{
  frontPointer = queue;     // Set queue pointers to start of buffer
  rearPointer = queue;
  news = 0;                 // Clear status flag
}


uint8_t uarttx_fifo_queue(uarttxfifoEl_t data)
{
  if (news == UARTTX_FIFO_SIZE)    // Test for Queue overflow
	{
		news = UARTTX_FIFO_OVERFLOW;   // Inform queue has overflowed
		return news;		
	}	

	*(rearPointer++) = data;	// Push data
	news++;				            // Update "news" counter

	if (rearPointer == (queue + UARTTX_FIFO_SIZE))	// If rear has exceded reset pointer
		rearPointer = queue;

	return news;			        // Inform Queue state
}


uarttxfifoEl_t uarttx_fifo_dequeue(void)
{
  uarttxfifoEl_t data;

	data = *(frontPointer++); // Pull data
	news--;                   // Update "news" counter

	if (frontPointer == queue + UARTTX_FIFO_SIZE)	// Check for Queue boundaries
		frontPointer = queue;		// If front has exceded reset pointer

	return data;              // Return retrieved data
}

uint8_t uarttx_fifo_getStatus(void)
{
  return news;
}

void uarttx_fifo_reset(void)
{
  uarttx_fifo_init();
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


/******************************************************************************/
