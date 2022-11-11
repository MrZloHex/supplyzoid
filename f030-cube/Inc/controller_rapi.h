/**
  ******************************************************************************
  * @file    controller_rapi.h 
  * @author  MrZloHex
  ******************************************************************************
  */

#ifndef __CONTROLLER_RAPI_H__
#define __CONTROLLER_RAPI_H__

#include "controller_types.h"

#include "usart.h"
#include "stdbool.h"

#define RAPI_BUF_LEN 32
#define RAPI_MAX_TOKENS 10

#define RAPI_SOC '$'
#define RAPI_EOC 0xD
#define RAPI_SOSI ':'

typedef struct Controller_RAPI_S
{
	UART_HandleTypeDef *uart;
	TIM_HandleTypeDef *tim;

	char accumulative_buffer[RAPI_BUF_LEN];
	char processive_buffer[RAPI_BUF_LEN];
	size_t acc_buf_index;

	char *tokens[RAPI_MAX_TOKENS];
	size_t token_index;

	bool msg_received;
	bool msg_processed;
} Controller_RAPI;

void
_controller_rapi_initialize
(
	Controller_RAPI *rapi,
	UART_HandleTypeDef *uart,
	TIM_HandleTypeDef *tim
);

Controller_Protocol_Result
_controller_rapi_start_recv(Controller_RAPI *rapi);

Controller_Protocol_Result
_controller_rapi_transfer(Controller_RAPI *rapi);

Controller_TaskResult
_controller_rapi_process(Controller_RAPI *rapi);

bool
_rapi_msg_validator(Controller_RAPI *rapi);

#endif /* __CONTROLLER_RAPI_H__ */

