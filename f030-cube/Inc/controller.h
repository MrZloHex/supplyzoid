/**
  ******************************************************************************
  * @file    controller.h 
  * @author  MrZloHex
  ******************************************************************************
  */

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "controller_queue.h"
#include "controller_types.h"
#include "controller_ocpp.h"
#include "controller_rapi.h"

#define MAX_QUEUE_CAPACITY 64

#define P_OCPP(controller) &(controller->ocpp) 
#define P_RAPI(controller) &(controller->rapi) 


typedef struct Controller_S
{
	Controller_Queue queue;
	Controller_OCPP ocpp;
	Controller_RAPI rapi;
} Controller;

Controller_Result
controller_initialize
(
	Controller *controller,
	UART_HandleTypeDef *ocpp_uart,
	UART_HandleTypeDef *rapi_uart,
	TIM_HandleTypeDef *ocpp_tim,
	TIM_HandleTypeDef *rapi_tim,
	RTC_HandleTypeDef *rtc
);

Controller_Result
controller_update(Controller *controller);

Controller_Queue_Result
controller_get_task(Controller *controller, Controller_Task *task);

Controller_Queue_Result
controller_set_task(Controller *controller, Controller_Task task);

#endif /* __CONTROLLER_H__ */
