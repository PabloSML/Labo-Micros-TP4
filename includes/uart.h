/***************************************************************************//**
  @file     uart.h
  @brief    UART Driver for K64F. Non-Blocking and using FIFO feature
  @author   Nicolás Magliola
 ******************************************************************************/

#ifndef _UART_H_
#define _UART_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "uart_config.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef enum{
  EVEN_PARITY = 0x00,
  ODD_PARITY = 0x01,
  NO_PARITY
} parity_t;

typedef struct {
    uint32_t baudrate;      // baudrate (0x0001 to 0x1FFF or default)
    uint8_t sbns;           // stop bit number select (1 or 2)
    uint8_t bitcant;        // 8-bit or 9-bit mode select (8 or 9)
    parity_t parity;        // Even, odd or off
} uart_cfg_t;

typedef enum{
  UART0_ID,
  UART1_ID,
  UART2_ID,
  UART3_ID,
  UART4_ID,
  UART5_ID
} uart_id_t;


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initialize UART driver
 * @param id UART's number
 * @param config UART's configuration (baudrate, stop bit num, bitcant, parity, etc.)
*/
void uartInit (uart_id_t id, uart_cfg_t config);

/**
 * @brief Check if a new byte was received
 * @param id UART's number
 * @return True if a new byte has been received
*/
bool uartIsRxMsg(uart_id_t id);

/**
 * @brief Check how many bytes were received
 * @param id UART's number
 * @return Quantity of received bytes
*/
uint8_t uartGetRxMsgLength(uart_id_t id);

/**
 * @brief Read a received message. Non-Blocking
 * @param id UART's number
 * @param msg Buffer to paste the received bytes
 * @param cant Desired quantity of bytes to be pasted
 * @return Real quantity of pasted bytes
*/
uint8_t uartReadMsg(uart_id_t id, uint8_t* msg, uint8_t cant);

/**
 * @brief Write a message to be transmitted. Non-Blocking
 * @param id UART's number
 * @param msg Buffer with the bytes to be transfered
 * @param cant Desired quantity of bytes to be transfered
 * @return Real quantity of bytes to be transfered
*/
uint8_t uartWriteMsg(uart_id_t id, const uint8_t* msg, uint8_t cant);

/**
 * @brief Check if all bytes were transfered
 * @param id UART's number
 * @return True if all bytes were transfered
*/
bool uartIsTxMsgComplete(uart_id_t id);


/*******************************************************************************
 ******************************************************************************/

#endif // _UART_H_
