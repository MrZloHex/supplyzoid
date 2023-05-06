/**
  ******************************************************************************
  * @file    controller_ocpp.h 
  * @author  MrZloHex
  ******************************************************************************
  */

#ifndef __CONTROLLER_OCPP_H__
#define __CONTROLLER_OCPP_H__

#include "controller_types.h"
#include "controller_ocpp_list.h"

#include "usart.h"
#include "stdbool.h"

#define VENDOR "EV Solutions"
// #define VENDOR "Mother"
#define MODEL  "PROTOTYPE"
// #define MODEL  "Fucker"


#define OCPP_BUF_LEN 512

#define ACTION_LEN   30
#define PAYLOAD_LEN  256
#define DSCR_LEN     255
#define ERR_CODE_LEN 30

#define MAX_RESPONSES 8

#define POS_MSG_TYPE	"$[0]"
#define POS_MSG_ID		"$[1]"
#define POS_CL_ACT    	"$[2]"
#define POS_CL_PAYLOAD	"$[3]"
#define POS_CR_PAYLOAD	"$[2]"
#define POS_CE_ERR_CODE	"$[2]"
#define POS_CE_ERR_DSCR	"$[3]"
#define POS_CE_ERR_DETL	"$[4]"

#define IS_MSG_TYPE(type) (type == CALL || type == CALLRESULT || type == CALLERROR)

typedef enum OCPP_MessageType_E
{
	CALL       = 2U,
	CALLRESULT = 3U,
	CALLERROR  = 4U
} OCPP_MessageType;

typedef enum OCPPAvailabilityStatus_E
{
	AS_Accepted	 = 0x0U,
	AS_Rejected  = 0x1U,
	AS_Scheduled = 0x2U
} OCPP_AvailabilityStatus;

typedef enum OCPPUpdateStatus_E
{
    US_Accepted	 = 0x0U,
    US_Failed	 = 0x1U,
    US_NotSupported = 0x2U,
	US_VersionMismatch = 0x3U
} OCPP_UpdateStatus;


#define IS_OCPP_OPERATIVE(__MEM__)	(__MEM__->status != CPS_Unavailable && __MEM__->status != CPS_Faulted)

typedef enum OCPPChargePointStatus_E
{
    CPS_Available       = 0x1U,
    CPS_Preparing       = 0x2U,
    CPS_Charging        = 0x3U,
    CPS_SuspendedEVSE   = 0x4U,
    CPS_SuspendedEV     = 0x5U,
    CPS_Finishing       = 0x6U,
    CPS_Reserved        = 0x7U,
    CPS_Unavailable     = 0x8U,
    CPS_Faulted         = 0x9U,
} OCPP_ChargePointStatus;

typedef enum OCPPChargePointErrorCode_E
{
	CPEC_NoError,
	CPEC_ConnectorLockFailure,
	CPEC_EVCommunicationError,
	CPEC_GroundFailure,
	CPEC_HighTemperature,
	CPEC_InternalError,
	CPEC_LocalListConflict,
	CPEC_OtherError,
	CPEC_OverCurrentFailure,
	CPEC_OverVoltage,
	CPEC_PowerMeterFailure,
	CPEC_PowerSwitchFailure,
	CPEC_ReaderFailure,
	CPEC_ResetFailure,
	CPEC_UnderVoltage,
	CPEC_WeakSignal
} OCPP_ChargePointErrorCode;

#ifndef	ID_LEN
#define ID_LEN		 64
typedef char OCPP_MessageID[ID_LEN];
#endif

typedef struct OCPP_Call_S
{
	OCPP_CallAction	action;
	char			payload[PAYLOAD_LEN]; // JSON
} OCPP_Call;

typedef struct OCPP_CallResult_S
{
	char			payload[PAYLOAD_LEN];
} OCPP_CallResult;

typedef struct OCPP_CallError_S
{
	OCPP_CallErrorCode	error_code;
	char				error_dscr[DSCR_LEN];
	char				error_details[PAYLOAD_LEN];
} OCPP_CallError;

typedef union OCPP_MessageData_U
{
	OCPP_Call		call;
	OCPP_CallResult	call_result;
	OCPP_CallError	call_error;
} OCPP_MessageData;

typedef struct OCPP_Message_S
{
	OCPP_MessageType	type;
	OCPP_MessageID 		id;
	OCPP_MessageData	data;
} OCPP_Message;

typedef struct OCPP_Expected_Message_S
{
	OCPP_MessageID id;
	OCPP_CallAction call_action;
} OCPP_Expected_Message;

#define OCPP_IdTag_Len 21
typedef char OCPP_IdTag[OCPP_IdTag_Len];

typedef struct Controller_OCPP_S
{
	UART_HandleTypeDef *uart;
	TIM_HandleTypeDef *tim;
	RTC_HandleTypeDef *rtc;

	char accumulative_buffer[OCPP_BUF_LEN];
	char processive_buffer[OCPP_BUF_LEN];
	size_t acc_buf_index;

	bool msg_received;
	bool msg_processed;

	OCPP_Message message;
	size_t id_msg;

	OCPP_IdTag idtag;

	uint32_t wh;

	OCPP_AuthorizationList list;

	bool is_response;
	OCPP_Message responses[MAX_RESPONSES];
	size_t q_resps;

	bool _started;

	Controller_Protocol_Result it_error;
} Controller_OCPP;

void
_controller_ocpp_initialize
(
	Controller_OCPP *ocpp,
	UART_HandleTypeDef *uart,
	TIM_HandleTypeDef *tim,
	RTC_HandleTypeDef *rtc,
	I2C_HandleTypeDef *i2c,
	GPIO_TypeDef   	  *wp_gpio,
	uint16_t 		  wp_pin
);

Controller_Protocol_Result
_controller_ocpp_transfer(Controller_OCPP *ocpp);

Controller_Protocol_Result
_controller_ocpp_process_income
(
	Controller_OCPP *ocpp,
	Controller_TaskWrap *wrap
);

Controller_Protocol_Result
_controller_ocpp_make_msg(Controller_OCPP *ocpp, OCPP_CallAction req, void *kwarg1, void *kwarg2);

Controller_Protocol_Result
_controller_ocpp_send_req(Controller_OCPP *ocpp, OCPP_CallAction req);

Controller_Protocol_Result
_controller_ocpp_send_resp
(
	Controller_OCPP *ocpp,
	OCPP_MessageType type,
	OCPP_MessageID id
);

bool
_ocpp_parse_msg(Controller_OCPP *ocpp);

bool
_ocpp_determine_message_type(Controller_OCPP *ocpp);

bool
_ocpp_get_message_id(Controller_OCPP *ocpp);

bool
_ocpp_get_action(Controller_OCPP *ocpp);

bool
_ocpp_get_payload(Controller_OCPP *ocpp, OCPP_MessageType type);

bool
_ocpp_get_call_error_code(Controller_OCPP *ocpp);

bool
_ocpp_get_call_error_descr(Controller_OCPP *ocpp);

void
_ocpp_set_id_msg(Controller_OCPP *ocpp);

size_t
_ocpp_get_id_resp(Controller_OCPP *ocpp);

Controller_Protocol_Result
_ocpp_append_resps(Controller_OCPP *ocpp);

void
_ocpp_delete_resp(Controller_OCPP *ocpp, size_t i);

bool
_ocpp_get_resp(Controller_OCPP *ocpp, size_t id);

#endif /* __CONTROLLER_OCPP_H__ */
