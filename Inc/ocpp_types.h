/**
  ******************************************************************************
  * @file    ocpp_types.h 
  * @author  MrZloHex
  ******************************************************************************
  */

#ifndef __OCPP_TYPES_H__
#define __OCPP_TYPES_H__

typedef enum OCPP_CallAction_E
{
	ACT_BOOT_NOTIFICATION			= 0x0U,
	ACT_START_TRANSACTION 			= 0x1U,
	ACT_STOP_TRANSACTION 			= 0x2U,
	ACT_STATUS_NOTIFICATION 		= 0x3U,
	ACT_METER_VALUES 				= 0x4U,
	ACT_HEARTBEAT 					= 0x5U,
	ACT_DATA_TRANSFER 				= 0x6U,
	ACT_AUTHORIZE 					= 0x7U,
	ACT_RESET 						= 0x8U,
	ACT_REMOTE_START_TRANSACTION	= 0x9U,
	ACT_REMOTE_STOP_TRANSACTION 	= 0xAU,
	ACT_CHANGE_AVAILABILITY 		= 0xBU,
	ACT_SEND_LOCAL_LIST 			= 0xCU,
	ACT_GET_LOCAL_LIST_VERSION 		= 0xDU
} OCPP_CallAction;

typedef enum OCPP_CallErrorCode_E
{
	ERC_NOT_IMPLEMENTED                = 1U,
	ERC_NOT_SUPPORTED                  = 2U,
	ERC_INTERNAL_ERROR                 = 3U,
	ERC_PROTOCOL_ERROR                 = 4U,
	ERC_SECURITY_ERROR                 = 5U,
	ERC_FORMATION_VIOLATION            = 6U,
	ERC_PROPERTY_CONSTRAINT_VIOLATION  = 7U,
	ERC_OCCURENCE_CONSTRAINT_VIOLATION = 8U,
	ERC_TYPE_CONSTRAINT_VIOLATION      = 9U,
	ERC_GENERIC_ERROR                  = 10U,
} OCPP_CallErrorCode;

#endif /* __OCPP_TYPES_H__ */