/**
  ******************************************************************************
  * @file    serial.c 
  * @author  MrZloHex
  * 
  * @brief   This file contains all the function implementation declared 
  *          in the serial.h file
  ******************************************************************************
  */

#include "serial.h"
#include "stdarg.h"

USART_Result
uprintf
(
	UART_HandleTypeDef *huart,
	uint32_t timeout,
	size_t size,
	const char * restrict format,
	...
)
{
	uint8_t msg[size];
	va_list args;
	va_start(args, format);
	int size_msg = vsnprintf
	(
		(char *)msg, size,
		format, args
	);
	va_end(args);

	if (size_msg < 0)
	{
		return USART_BUFFER_OVERFLOW;
	}

	return (USART_Result) HAL_UART_Transmit(huart, msg, size_msg, timeout);

}


USART_Result
uread
(
    UART_HandleTypeDef *huart,
	uint32_t timeout,
    char *msg,
	size_t msg_size,
	const char stop_char
)
{
	size_t index = 0;
	uint8_t ch = 0;
	while (1)
	{
		HAL_StatusTypeDef result = HAL_UART_Receive(huart, &ch, 1, timeout);

		if (result != HAL_OK)
		{
			return (USART_Result) result;
		}

		if (index >= msg_size)
		{
			return USART_BUFFER_OVERFLOW;
		}

		if ((char)ch == stop_char)
		{
			msg[index] = (char)0;
			return USART_IO_OK;
		}
		
		msg[index++] = (char)ch;
	}

	return USART_IO_ERROR;
}
