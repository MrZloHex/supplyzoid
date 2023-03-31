/**
  ******************************************************************************
  * @file    controller.h 
  * @author  MrZloHex
  ******************************************************************************
  */

#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "controller_taskset.h"
#include "controller_ocpp.h"
#include "controller_rapi.h"

#include "lcd_i2c.h"

#define MAX_TASKSET_CAPACITY 16

#define P_OCPP(controller) &(controller->ocpp) 
#define P_RAPI(controller) &(controller->rapi) 


typedef struct Controller_S
{
	Controller_TaskSet task_set;
	Controller_OCPP ocpp;
	Controller_RAPI rapi;
	LCD_I2C 	lcd;
} Controller;

Controller_Result
controller_initialize
(
	Controller *controller,
	UART_HandleTypeDef *ocpp_uart,
	UART_HandleTypeDef *rapi_uart,
	TIM_HandleTypeDef *ocpp_tim,
	TIM_HandleTypeDef *rapi_tim,
	RTC_HandleTypeDef *rtc,
	I2C_HandleTypeDef *i2c,
	GPIO_TypeDef   	  *wp_gpio,
	uint16_t 	   wp_pin
);

Controller_Result
controller_update(Controller *controller);

#endif /* __CONTROLLER_H__ */
