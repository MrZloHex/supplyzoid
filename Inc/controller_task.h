/**
  ******************************************************************************
  * @file    controller_task.h 
  * @author  MrZloHex
  ******************************************************************************
  */

#ifndef __CONTROLLER_TASK_H__
#define __CONTROLLER_TASK_H__

#include "stdlib.h"
#include "stdint.h"

struct Controller_S;

typedef enum Controller_TaskType_E
{
	TASK_TRIGGER = 0x0U,
	TASK_PROCESS = 0x1U,
	TASK_TIMEOUT = 0x2U
} Controller_TaskType;

typedef struct Task_Result_S Task_Result;

#ifndef	ID_LEN
#define ID_LEN		 64
typedef char OCPP_MessageID[ID_LEN];
#endif

typedef Task_Result (*Ptr_Task)(struct Controller_S *, OCPP_MessageID);

typedef enum MsgExpect_USART_E
{
	RAPI_USART = 0x0U,
	OCPP_USART = 0x1U
} MsgExpect_USART;

typedef struct Controller_Task_S
{
	Controller_TaskType	type;
	size_t			id;
	OCPP_MessageID		trigger_id;
	MsgExpect_USART 	usart;
	Ptr_Task		func;
	Ptr_Task		func_timeout;
	uint32_t 		genesis_time;
} Controller_Task;

typedef enum Controller_WrapType_E
{
	WRAP_EMPTY,
	WRAP_IN_PROGRESS,
	WRAP_FINISHED,
	WRAP_TIMEOUT
} Controller_WrapType;

typedef struct Controller_TaskWrap_S
{
	Controller_WrapType 	type;
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
