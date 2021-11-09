/***************************************************************************//**
  @file     uart.c
  @brief    UART Driver for K64F. Non-Blocking and using FIFO feature
  @author   Grupo 4
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "uart.h"
#include "uartrxfifo.h"
#include "uarttxfifo.h"
#include "board.h"
#include "hardware.h"
#include "MK64F12.h"
#include "tools.h"

#if UART_DEVELOPEMENT_MODE
#include <stdio.h>
#endif

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

// Clock Gating Masks
#define UART_CG_MASKS {SIM_SCGC4_UART0_MASK, SIM_SCGC4_UART1_MASK, SIM_SCGC4_UART2_MASK, SIM_SCGC4_UART3_MASK, SIM_SCGC1_UART4_MASK, SIM_SCGC1_UART5_MASK}

// UART TX and RX PINS
#define UART_PINS {{UART0_RX_PIN, UART0_TX_PIN}, {UART1_RX_PIN, UART1_TX_PIN}, {UART2_RX_PIN, UART2_TX_PIN}, {UART3_RX_PIN, UART3_TX_PIN}, {UART4_RX_PIN, UART4_TX_PIN}, {UART5_RX_PIN, UART5_TX_PIN}}

// UART TX and RX IRQs
#define UART_IRQS {UART0_RX_TX_IRQn, UART1_RX_TX_IRQn, UART2_RX_TX_IRQn, UART3_RX_TX_IRQn, UART4_RX_TX_IRQn, UART5_RX_TX_IRQn}

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/



/*******************************************************************************
 * VARIABLES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


static void uartSetBaudRate(uart_id_t id, uint32_t baudrate);


/*******************************************************************************
 * ROM CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * STATIC VARIABLES AND CONST VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/

static PORT_Type * portpointer[] = PORT_BASE_PTRS;
static UART_Type * uartpointers[] = UART_BASE_PTRS;
static uint16_t uart_cg_masks[] = UART_CG_MASKS;
static uint8_t uart_rxtx_pins[][2] = UART_PINS;
static uint8_t uart_rxtx_irqs[] = UART_IRQS;
static bool yaInits[UART_CANT_IDS] = {false};

static uint8_t news = 0;
static uint8_t myNews = 0;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


void uartInit (uart_id_t id, uart_cfg_t config)
{
  if (!(yaInits[id]))
  {
    if (id <= UART3_ID)                     // clock gating
      SIM->SCGC4 |= uart_cg_masks[id];
    else if (id <= UART5_ID)
      SIM->SCGC1 |= uart_cg_masks[id];

    uartSetBaudRate(id, config.baudrate);   // baudrate set

    uartpointers[id]->BDH |= UART_BDH_SBNS(config.sbns-1);  // stop bit num
    uartpointers[id]->C1 |= UART_C1_M(config.bitcant-8);    // 8 or 9 bit mode
    if (config.parity != NO_PARITY)
    {
      uartpointers[id]->C1 |= UART_C1_PE(HIGH);
      uartpointers[id]->C1 |= UART_C1_PT(config.parity);    // parity fun
    }

    // uint8_t txFifoSize = uartpointers[id]->PFIFO & UART_PFIFO_TXFIFOSIZE_MASK; UART3 = 1 dataword
    // uint8_t rxFifoSize = uartpointers[id]->PFIFO & UART_PFIFO_RXFIFOSIZE_MASK; UART3 = 1 dataword

    uint32_t port, numpin;

    for(uint8_t i = 0; i < 2; i++)
    {
      port = PIN2PORT(uart_rxtx_pins[id][i]);
      numpin = PIN2NUM(uart_rxtx_pins[id][i]);
      
      portpointer[port]->PCR[numpin] = 0x0; // Clear all bits
      portpointer[port]->PCR[numpin] |= PORT_PCR_MUX(PORT_mAlt3); // Set MUX to UART
      portpointer[port]->PCR[numpin] |= PORT_PCR_IRQC(PORT_eDisabled); // Disable Port Interrupts
    }

    uartpointers[id]->C2 = UART_C2_TE_MASK | UART_C2_RE_MASK | UART_C2_RIE_MASK;
    NVIC_EnableIRQ(uart_rxtx_irqs[id]);   // Enable RX-TX IRQ

    uartrx_fifo_init();   // Only one uart used in proyect (Need multiple fifos if more!!)
    uarttx_fifo_init();

    yaInits[id] = true;
  }
}

bool uartIsRxMsg(uart_id_t id)
{
  return (bool)(uartrx_fifo_getStatus());
}

uint8_t uartGetRxMsgLength(uart_id_t id)
{
  return uartrx_fifo_getStatus(); // Potentially change if message processing
}

uint8_t uartReadMsg(uart_id_t id, uint8_t* msg, uint8_t cant)
{

// #if UART_DEVELOPEMENT_MODE
//   if(news == UARTRX_FIFO_OVERFLOW)
//     printf("ERROR: MISSED DATA!");
// #endif

  uint8_t i, realCant = 0;
  for(i = 0; i < cant; i++)
  {
    if(uartrx_fifo_getStatus())
    {
      *(msg++) = uartrx_fifo_dequeue();
      realCant++;
    }
    else
      break; // If FIFO is empty stop read loop
  }
  return realCant;
}

uint8_t uartWriteMsg(uart_id_t id, const uint8_t* msg, uint8_t cant)
{
  uint8_t i, queueOccup, realCant = 0;
  for(i = 0; i < cant; i++)
  {
    queueOccup = uarttx_fifo_queue(*(msg++));
    if(queueOccup != UARTTX_FIFO_OVERFLOW)
      realCant++;
    else
      break;
  }

  uartpointers[id]->C2 |= UART_C2_TIE_MASK; // Raise Transmit Interrupt Enable flag

  return realCant;
}

bool uartIsTxMsgComplete(uart_id_t id)
{
  return (bool)((uartpointers[id]->S1 & UART_S1_TDRE_MASK) && (!(uarttx_fifo_getStatus()))); // HW and SW buffers empty
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


static void uartSetBaudRate(uart_id_t id, uint32_t baudrate)
{
  uint16_t sbr, brfa;
  uint32_t clock;
  UART_Type* uart = uartpointers[id];

  clock = ((uart == UART0) || (uart == UART1)) ? (__CORE_CLOCK__):(__CORE_CLOCK__ >> 1);

  // if ((baudrate == 0) || (baudrate > 0x1FFF))
  //   baudrate = UART_HAL_DEFAULT_BAUDRATE;

  sbr = clock / (baudrate << 4);                // sbr = clock / (baudrate x 16)
  brfa = (clock << 1) / baudrate - (sbr << 5);  // brfa = 2*clock/baudrate - 32*sbr

  uart->BDH = UART_BDH_SBR(sbr >> 8);
  uart->BDL = UART_BDL_SBR(sbr);
  uart->C4 = (uart->C4 & ~UART_C4_BRFA_MASK) | UART_C4_BRFA(brfa);
}

// Copy and paste funs for "fast and dirty" irq


#ifdef UART0_CONFIG
__ISR__ UART0_RX_TX_IRQHandler(void)
{

  if(UART0->S1 & UART_S1_RDRF_MASK) // Data recieved
  {
    news = uartrx_fifo_queue(UART0->D);
  }

  if((UART0->S1 & UART_S1_TDRE_MASK)) // Transmission Available
  {
    myNews = uarttx_fifo_getStatus();
    if(myNews)
    {
      UART0->D = uarttx_fifo_dequeue();
      if(myNews == 1)
        UART0->C2 &= ~(UART_C2_TIE_MASK);   // "El ultimo apaga la luz"
    }
      
  }
}
#endif

#ifdef UART3_CONFIG
__ISR__ UART3_RX_TX_IRQHandler(void)
{

  if(UART3->S1 & UART_S1_RDRF_MASK) // Data recieved
  {
    news = uartrx_fifo_queue(UART3->D);
  }

  if((UART3->S1 & UART_S1_TDRE_MASK)) // Transmission Available
  {
    myNews = uarttx_fifo_getStatus();
    if(myNews)
    {
      UART3->D = uarttx_fifo_dequeue();
      if(myNews == 1)
        UART3->C2 &= ~(UART_C2_TIE_MASK);   // "El ultimo apaga la luz"
    }
      
  }
}
#endif


/******************************************************************************/
