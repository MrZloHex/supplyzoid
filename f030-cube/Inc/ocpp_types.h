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
	ACT_BOOT_NOTIFICATION        = 1U,
	ACT_REMOTE_START_TRANSACTION = 2U,
	ACT_REMOTE_STOP_TRANSACTION  = 3U,
	ACT_START_TRANSACTION        = 4U,
	ACT_STOP_TRANSACTION         = 5U,
	ACT_STATUS_NOTIFICATION      = 6U,
	ACT_METER_VALUES             = 7U,
	ACT_HEARTBEAT                = 8U,
	ACT_DATA_TRANSFER            = 9U,
	ACT_RESET                    = 10U,
	ACT_AUTHORIZE                = 11U,
	ACT_CHANGE_AVAILABILITY      = 12U
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