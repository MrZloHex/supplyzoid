/**
  ******************************************************************************
  * @file    controller_types.h 
  * @author  MrZloHex
  ******************************************************************************
  */

#ifndef __CONTROLLER_TYPES_H__
#define __CONTROLLER_TYPES_H__

#define DEBUG

#include "ocpp_types.h"
#include "stdbool.h"

typedef enum Controller_ResultType_E
{
	CTRL_OK                 = 0x0U,
	CTRL_ERR                = 0x1U,
	CTRL_QUEUE_ERR			= 0x2U,
	CTRL_OCPP_ERR 			= 0x3U,
	CTRL_RAPI_ERR 			= 0x4U,
	CTRL_TASK_ERR			= 0x5U
} Controller_ResultType;

typedef enum Controller_Queue_Result_E
{
	CTRL_QUE_OK,
	CTRL_QUE_ALLOC_ERR,
	CTRL_QUE_OVERFLOW,
	CTRL_QUE_EMPTY,
	CTRL_QUE_NULLPTR
} Controller_Queue_Result;

typedef enum Controller_Protocol_Result_E
{
	CTRL_PTCL_OK			= 0x0U,
	CTRL_PTCL_HAL_ERROR		= 0x1U,
	CTRL_PTCL_HAL_BUSY		= 0x2U,
	CTRL_PTCL_HAL_TIMEOUT	= 0x3U,
	CTRL_PTCL_ACC_BUF_FULL	= 0x4U,
	CTRL_PTCL_PRC_BUF_FULL	= 0x5U,
	CTRL_PTCL_ACC_BUF_EMPT  = 0x6U,
	CTRL_PTCL_PRC_BUF_EMPT  = 0x7U,
	CTRL_PTCL_NON_VALID_MSG	= 0x8U,
	CTRL_PTCL_UNKNOWN_MSG	= 0x9U,
	CTRL_PTCL_NO_SUCH_MSG	= 0x10U
} Controller_Protocol_Result;

typedef union Controller_Errors_U
{
	Controller_Queue_Result		queue_err;
	Controller_Protocol_Result	ocpp_err;
	Controller_Protocol_Result	rapi_err;
} Controller_Errors;

typedef struct Controller_Result_S
{
	Controller_ResultType	type;
	Controller_Errors 		errors;
} Controller_Result;



typedef enum Controller_TaskType_E
{
	TASK_RAPI_GET_MSG			= 0x0U,
	TASK_RAPI_PROC_MSG			= 0x1U,
	TASK_RAPI_TRANSFER_MSG		= 0x2U,
	TASK_OCPP_GET_MSG			= 0x3U,
	TASK_OCPP_PROC_MSG			= 0x4U,
	TASK_OCPP_TRANSFER_MSG		= 0x5U,
	TASK_OCPP_MAKE_REQ			= 0x6U,
	TASK_OCPP_SEND_REQ			= 0X7U,
	NO_TASK						= 0x8U
} Controller_TaskType;

typedef struct Task_OCPP_MakeReq_S
{
	OCPP_CallAction action;
} Task_OCPP_MakeReq;

typedef struct Task_OCPP_SendReq_S
{
	OCPP_CallAction action;
} Task_OCPP_SendReq;

typedef union Task_Data_U
{
	Task_OCPP_MakeReq ocpp_make_req;
	Task_OCPP_SendReq ocpp_send_req;
} Task_Data;

typedef struct Controller_Task_S
{
	Controller_TaskType type;
	Task_Data 			data;
} Controller_Task;

typedef struct Controller_TaskResult_S
{
	Controller_ResultType	type;
	Controller_Errors 		errors;
	Controller_Task 		task;
} Controller_TaskResult;



#define CONTROLLER_OKAY													Controller_Result __res =						\
																		{												\
																			.type = CTRL_OK								\
																		};												\
																		return __res;

#define CONTROLLER_ERROR(__err_type__, __err_field__, __error__) 		Controller_Result __res = 						\
																		{												\
																			.type = __err_type__,						\
																			.errors = { .__err_field__ = __error__ }	\
																		};												\
																		return __res;


#define CONTROLLER_OCPP_ERROR(__error__)								CONTROLLER_ERROR(CTRL_OCPP_ERR, ocpp_err, __error__)
#define CONTROLLER_RAPI_ERROR(__error__)								CONTROLLER_ERROR(CTRL_RAPI_ERR, rapi_err, __error__)

#define CONTROLLER_TASK_RESULT(__task__) 								Controller_TaskResult __res =					\
																		{												\
																			.type = CTRL_OK, .task = __task__			\
																		};												\
																		return __res;

#define CONTROLLER_TASK_ERROR(__err_type__, __err_field__, __error__)	Controller_TaskResult __res =					\
																		{												\
																			.type = __err_type__,						\
																			.errors = { .__err_field__ = __error__ }	\
																		};												\
																		return __res;

#define CONTROLLER_TASK_OCPP_ERROR(__error__)							CONTROLLER_TASK_ERROR(CTRL_OCPP_ERR, ocpp_err, __error__)
#define CONTROLLER_TASK_RAPI_ERROR(__error__)							CONTROLLER_TASK_ERROR(CTRL_RAPI_ERR, rapi_err, __error__)

#endif /* __CONTROLLER_TYPES_H__ */