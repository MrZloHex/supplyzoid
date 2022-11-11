/**
  ******************************************************************************
  * @file    controller_types.h 
  * @author  MrZloHex
  ******************************************************************************
  */

#ifndef __CONTROLLER_TYPES_H__
#define __CONTROLLER_TYPES_H__

typedef enum Controller_ResultType_E
{
	CTRL_OK                 = 0x0U,
	CTRL_ERR                = 0x1U,
	CTRL_QUEUE_ERR			= 0x2U,
	CTRL_OCPP_ERR 			= 0x3U,
	CTRL_RAPI_ERR 			= 0x4U
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
} Controller_Protocol_Result;

typedef union Controller_Errors_U
{
	Controller_Queue_Result queue_err;
	Controller_Protocol_Result ocpp_err;
	Controller_Protocol_Result rapi_err;
} Controller_Errors;

typedef struct Controller_Result_S
{
	Controller_ResultType	type;
	Controller_Errors 		errors;
} Controller_Result;

#define CONTROLLER_OKAY												Controller_Result __res =						\
																	{												\
																		.type = CTRL_OK								\
																	};												\
																	return __res;

#define CONTROLLER_ERROR(__err_type__, __err_field__, __error__) 	Controller_Result __res = 						\
																	{												\
																		.type = __err_type__,						\
																		.errors = { .__err_field__ = __error__ }	\
																	};												\
																	return __res;


#define CONTROLLER_OCPP_ERROR(__error__)	CONTROLLER_ERROR(CTRL_OCPP_ERR, ocpp_err, __error__)
#define CONTROLLER_RAPI_ERROR(__error__)	CONTROLLER_ERROR(CTRL_RAPI_ERR, rapi_err, __error__)



typedef enum Controller_Task_E
{
	TASK_GET_MSG_RAPI			= 0x0U,
	TASK_PROC_MSG_RAPI			= 0x1U,
	TASK_TRANSFER_MSG_RAPI		= 0x2U,
	TASK_GET_MSG_OCPP			= 0x3U,
	TASK_PROC_MSG_OCPP			= 0x4U,
	TASK_TRANSFER_MSG_OCPP		= 0x5U,
	NO_TASK						= 0x6U
} Controller_Task;

#endif /* __CONTROLLER_TYPES_H__ */