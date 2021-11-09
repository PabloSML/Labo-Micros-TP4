/***************************************************************************//**
  @file     App.c template
  @brief    Application functions
  @author   Grupo 4
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "board.h"
#include "hardware.h"
#include "MK64F12.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/



/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
  hw_DisableInterrupts();

  //Enable clocking for port B,A,E
  SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
  SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

  PORT_Type * portpointer[] = PORT_BASE_PTRS;
  portpointer[PA]->ISFR |= 0xFFFFU;
  
  NVIC_EnableIRQ(PORTA_IRQn);

  ledInit(LED_RED);
  switchInit(SW3);

  irq_id_t id = irqGetId(SW3);
  gpioIRQ(SW3, PORT_eInterruptFalling, id, &ledToggle);

  hw_EnableInterrupts();

}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
  while (1)
  {
    /* wait... */
  }
  
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



/*******************************************************************************
 ******************************************************************************/