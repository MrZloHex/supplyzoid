/**
  ******************************************************************************
  * @file    serial.h 
  * @author  MrZloHex
  * 
  * @brief   This file contains all the function prototypes and type
  *          definitions for the serial.c file
  ******************************************************************************
  */

#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "stdio.h"
#include "usart.h"	

typedef enum USART_Result_E
{
	USART_IO_OK				= 0x00U,
	USART_IO_ERROR			= 0x01U,
	USART_IO_BUSY			= 0x02U,
	USART_IO_TIMEOUT		= 0x03U,
	USART_BUFFER_OVERFLOW	= 0x04U,
} USART_Result;

/**
 * @brief Prints to USART with format
 * 
 * @param huart 	UART Handle
 * @param timeout 	Timeout duration
 * @param size 		Maximum size of message
 * @param format 	Format string
 * @param ... 		va_list of formats
 * @return USART_Result 
 */
USART_Result
uprintf
(
	UART_HandleTypeDef *huart,
	uint32_t timeout,
	size_t size,
	const char * restrict format,
	...
);

/**
 * @brief Reads from USART
 * 
 * @param huart 	UART Hanle
 * @param timeout 	Timeout duration
 * @param msg 		Ptr to data buffer
 * @param msg_size 	Size of buffer
 * @param stop_char Char up to which read
 * @return USART_Result 
 */
USART_Result
uread
(
	UART_HandleTypeDef *huart,
	uint32_t timeout,
	char *msg,
	size_t msg_size,
	const char stop_char
);

#endif /* __SERIAL_H__ */
