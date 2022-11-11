/**
  ******************************************************************************
  * @file    controller_ocpp.h 
  * @author  MrZloHex
  ******************************************************************************
  */

#ifndef __CONTROLLER_OCPP_H__
#define __CONTROLLER_OCPP_H__

#include "controller_types.h"

#include "usart.h"
#include "stdbool.h"

#define OCPP_BUF_LEN 512

typedef struct Controller_OCPP_S
{
	UART_HandleTypeDef *uart;
	TIM_HandleTypeDef *tim;
	RTC_HandleTypeDef *rtc;

	char accumulative_buffer[OCPP_BUF_LEN];
	char processive_buffer[OCPP_BUF_LEN];
	size_t acc_buf_index;

	bool msg_received;
	bool msg_processed;
} Controller_OCPP;

void
_controller_ocpp_initialize
(
	Controller_OCPP *ocpp,
	UART_HandleTypeDef *uart,
	TIM_HandleTypeDef *tim,
	RTC_HandleTypeDef *rtc
);

Controller_Protocol_Result
_controller_ocpp_start_recv(Controller_OCPP *ocpp);

Controller_Protocol_Result
_controller_ocpp_transfer(Controller_OCPP *ocpp);

void
_controller_ocpp_process(Controller_OCPP *ocpp);

#endif /* __CONTROLLER_OCPP_H__ */
