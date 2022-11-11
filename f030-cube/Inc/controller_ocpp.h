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

// #define VENDOR "EV Solutions"
#define VENDOR "Mother"
// #define MODEL  "PROTOTYPE"
#define MODEL  "Fucker"


#define OCPP_BUF_LEN 512

#define ACTION_LEN   30
#define PAYLOAD_LEN  256
#define ID_LEN		 64
#define DSCR_LEN     255
#define ERR_CODE_LEN 30


typedef enum OCPP_MessageType_E
{
	CALL       = 2U,
	CALLRESULT = 3U,
	CALLERROR  = 4U
} OCPP_MessageType;

typedef char OCPP_MessageID[ID_LEN];

typedef struct OCPP_Call_S
{
	OCPP_CallAction	action;
	char			payload[PAYLOAD_LEN]; // JSON
} OCPP_Call;

typedef struct OCPP_CallResult_S
{
	char			payload[PAYLOAD_LEN];
} OCPP_CallResult;

typedef struct OCPP_CallError_S
{
	OCPP_CallErrorCode	error_code;
	char				error_dscr[DSCR_LEN];
	char				error_details[PAYLOAD_LEN];
} OCPP_CallError;

typedef union OCPP_MessageData_U
{
	OCPP_Call		call;
	OCPP_CallResult	call_result;
	OCPP_CallError	call_error;
} OCPP_MessageData;

typedef struct OCPP_Message_S
{
	OCPP_MessageType	type;
	OCPP_MessageID 		id;
	OCPP_MessageData	data;
} OCPP_Message;

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

	OCPP_Message message;
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

void
_controller_ocpp_make_req(Controller_OCPP *ocpp, Task_OCPP_MakeReq req);

#endif /* __CONTROLLER_OCPP_H__ */
