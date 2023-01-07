/**
  ******************************************************************************
  * @file    controller_task.h 
  * @author  MrZloHex
  ******************************************************************************
  */

#ifndef __CONTROLLER_TASK_H__
#define __CONTROLLER_TASK_H__

#include "stdlib.h"

struct Controller_S;

typedef enum Controller_TaskType_E
{
	TASK_TRIGGER = 0x0U,
	TASK_PROCESS = 0x1U
} Controller_TaskType;

typedef struct Task_Result_S Task_Result;

typedef Task_Result (*Ptr_Task)(struct Controller_S *);

typedef enum MsgExpect_USART_E
{
	RAPI_USART = 0x0U,
	OCPP_USART = 0x1U
} MsgExpect_USART;

typedef struct Controller_Task_S
{
	Controller_TaskType	type;
	size_t				id;
	MsgExpect_USART 	usart;
	Ptr_Task			func;
} Controller_Task;

typedef enum Controller_WrapType_E
{
	WRAP_EMPTY,
	WRAP_IN_PROGRESS,
	WRAP_FINISHED
} Controller_WrapType;

typedef struct Controller_TaskWrap_S
{
	Controller_WrapType type;
	Controller_Task		task;
} Controller_TaskWrap;

struct Task_Result_S
{
	enum Task_ResultType_E
	{
		TRES_NEXT,
		TRES_WAIT
	} type;
	Controller_TaskWrap task;
};

#endif /* __CONTROLLER_TASK_H__ */