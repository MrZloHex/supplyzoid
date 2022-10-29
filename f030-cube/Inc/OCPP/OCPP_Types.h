#ifndef __OCPP_TYPES_H__
#define __OCPP_TYPES_H__

#include "stdbool.h"
#include "stdint.h"
#include "usart.h"
#include "OCPP/OCPP_States.h"


#define OCPP_BUF_LEN 512


typedef enum OCPPResult_E
{
	RES_ERROR = 0U,
	RES_OK    = 1U
} OCPPResult;

typedef char CiString20[21];
typedef char CiString25[26];

#define ACTION_LEN   30
#define PAYLOAD_LEN  256
#define ID_LEN		 64
#define DSCR_LEN     255
#define ERR_CODE_LEN 30
#define REQ_LEN 	 512


#define POS_MSG_TYPE "$[0]"
#define POS_MSG_ID   "$[1]"

#define IS_MSG_TYPE(type) (type == CALL || type == CALLRESULT || type == CALLERROR)

typedef enum OCPPMessageType_E
{
	CALL       = 2U,
	CALLRESULT = 3U,
	CALLERROR  = 4U
} OCPPMessageType;


typedef char * OCPPMessageID;




// [2,"1","BootNotification",{"chargingStation":{"model":"SingleSocketCharger","vendorName":"VendorX"}}]
#define POS_CL_ACT     "$[2]"
#define POS_CL_PAYLOAD "$[3]"
typedef struct
{
	OCPPCallAction action;
	char          *payload; // JSON
} OCPPCall;




// [3,"1",{"currentTime":"2013-02-01T20:53:32.486Z","interval":3,"status":"Accepted"}]
#define POS_CR_PAYLOAD "$[2]"

typedef struct
{
	char        *payload;
} OCPPCallResult;



// [4,"19223201","GenericError","Any other error not covered by the previous ones",{}]
#define POS_CE_ERR_CODE "$[2]"
#define POS_CE_ERR_DSCR "$[3]"
#define POS_CE_ERR_DETL "$[4]"

typedef struct
{
	OCPPCallErrorCode error_code;
	char        	  *error_dscr;
	char        	  *error_details;
} OCPPCallError;


typedef struct OCPPMessage_S
{
	OCPPMessageType type;
	OCPPMessageID   ID;
	OCPPCall       	call;
	OCPPCallResult 	call_result;
	OCPPCallError  	call_error;
} OCPPMessage;

/**
 * @brief Main OCPP Type
 * 
 * @param _transaction_id 	Transaction ID
 * @param _id 				ID of OCPP message
 * @param _wait_resp		Flag which indicates if Server should send response, not a request
 * @param _booted 			Flag of main boot of device
 * 
 * @param pres_msg		Present OCPP message
 * @param last_msg		Previous OCPP message
 * 
 */
typedef struct OCPP_S
{
	uint32_t _transaction_id;
	uint32_t _id;
	bool _wait_resp;
	bool _is_transaction;
	uint32_t millis;
	bool _booted;

	OCPPMessage pres_msg;
	OCPPMessage last_msg;

	UART_HandleTypeDef *uart;
	RTC_HandleTypeDef *rtc;

	char buffer[OCPP_BUF_LEN];
	size_t buf_i;
	bool got_msg;

	char cmd_buf[OCPP_BUF_LEN];
	bool proc_msg;
} OCPP;

#endif /* __OCPP_TYPES_H__ */