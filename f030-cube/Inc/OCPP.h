// /**
//   ******************************************************************************
//   * @file    OCPP.h 
//   * @author  MrZloHex
//   * 
//   * @brief   This file contains all the function prototypes and
//   *          for the OCPP.c file
//   ******************************************************************************
//   */
// 
// 
#ifndef __OCPP_H__
#define __OCPP_H__
// 
// // #define VENDOR "EV Solutions"
// #define VENDOR "Mother"
// // #define MODEL  "PROTOTYPE"
// #define MODEL  "Fucker"
// 
// #include "string.h"
// 
#include "RAPI/RAPI_Types.h"
#include "OCPP/OCPP_Types.h"
// 
// 
// 
// 
// void
// ocpp_init
// (
// 	OCPP *ocpp,
// 	UART_HandleTypeDef *uart,
// 	RTC_HandleTypeDef *rtc
// );
// 
// void
// ocpp_deinit(OCPP *ocpp);
// 
// void
// ocpp_next(OCPP *ocpp);
// 
// void
// ocpp_update
// (
// 	OCPP *ocpp,
// 	RAPI *rapi
// );
// 
// void
// ocpp_handle_message
// (
//  	OCPP *ocpp,
// 	RAPI *rapi
// );
// 
// OCPPResult
// ocpp_parse_message(OCPP *ocpp);
// 
// 
// void
// ocpp_send_req
// (
// 	OCPP *ocpp,
// 	OCPPCallAction action
// );
// 
// void
// ocpp_send_resp
// (
// 	OCPP *ocpp,
// 	OCPPMessageType type
// );
// 
// 
// 
// 
// 
// 
// 
// 
// OCPPResult
// ocpp_determine_message_type(OCPP *ocpp);
// 
// OCPPResult
// ocpp_get_message_id(OCPP *ocpp);
// 
// OCPPResult
// ocpp_get_action(OCPP *ocpp);
// 
// OCPPResult
// ocpp_get_payload
// (
// 	OCPP *ocpp,
// 	OCPPMessageType type
// );
// 
// OCPPResult
// ocpp_get_call_error_code(OCPP *ocpp);
// 
// OCPPResult
// ocpp_get_call_error_descr(OCPP *ocpp);
// 
// void
// ocpp_set_msg_id(OCPP *ocpp);
// 
#endif /* __OCPP_H__ */