#include "controller_ocpp.h"
#include "controller_ocpp_msg.h"

#include "string.h"
#include "serial.h"
#include "mjson.h"
#include "convert.h"

#include "task_sequences/remote_start_sequence/rss_task_1.h"
#include "task_sequences/remote_stop_sequence/rsts_task_1.h"
#include "task_sequences/availability_sequence/ca_task_1.h"
#include "task_sequences/reset_sequence/rt_task_1.h"
#include "task_sequences/send_local_sequence/sll_task_1.h"
#include "task_sequences/get_verlist_sequence/gvl_task_1.h"
#include "task_sequences/data_transfer_sequence/dt_task_1.h"


const static char	k_ACT_BOOT_NOTIFICATION[]	= "BootNotification";
const static char	k_ACT_START_TRANSACTION[]	= "StartTransaction";
const static char	k_ACT_STOP_TRANSACTION[]         = "StopTransaction";
const static char	k_ACT_STATUS_NOTIFICATION[]      = "StatusNotification";
const static char	k_ACT_METER_VALUES[]             = "MeterValues";
const static char	k_ACT_HEARTBEAT[]                = "Heartbeat";
const static char	k_ACT_DATA_TRANSFER[]            = "DataTransfer";
const static char	k_ACT_AUTHORIZE[]                = "Authorize";
const static char 	k_ACT_RESET[]				     = "Reset";
const static char	k_ACT_REMOTE_START_TRANSACTION[] = "RemoteStartTransaction";
const static char	k_ACT_REMOTE_STOP_TRANSACTION[]	 = "RemoteStopTransaction";
const static char	k_ACT_CHANGE_AVAILABILITY[]		 = "ChangeAvailability";
const static char	k_ACT_SEND_LOCAL_LIST[]			 = "SendLocalList";
const static char 	k_ACT_GET_LOCAL_LIST_VERSION[]	 = "GetLocalListVersion";
const static char  *k_ACTIONS_STRINGS[14] =
{
	k_ACT_BOOT_NOTIFICATION, k_ACT_START_TRANSACTION, k_ACT_STOP_TRANSACTION,
	k_ACT_STATUS_NOTIFICATION, k_ACT_METER_VALUES, k_ACT_HEARTBEAT,
	k_ACT_DATA_TRANSFER, k_ACT_AUTHORIZE, k_ACT_RESET,
 	k_ACT_REMOTE_START_TRANSACTION, k_ACT_REMOTE_STOP_TRANSACTION, k_ACT_CHANGE_AVAILABILITY,
	k_ACT_SEND_LOCAL_LIST, k_ACT_GET_LOCAL_LIST_VERSION
};


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
)
{
    ocpp->uart = uart;
	ocpp->tim = tim;
    ocpp->rtc = rtc;

	memset(ocpp->accumulative_buffer, 0, OCPP_BUF_LEN);
	memset(ocpp->processive_buffer,   0, OCPP_BUF_LEN);
	ocpp->acc_buf_index = 0;

	ocpp->msg_received = false;
	ocpp->msg_processed = true;

	ocpp->id_msg = 1;
	ocpp->is_response = false;
	ocpp->q_resps = 0;

	ocpp->wh = 0;

	// ocpp_authlist_init(&(ocpp->list), i2c, wp_gpio, wp_pin);

	ocpp->_started = false;

	ocpp->it_error = CTRL_PTCL_OK;
}

Controller_Protocol_Result
_controller_ocpp_transfer(Controller_OCPP *ocpp)
{
	if (!ocpp->msg_received)
	{
		return CTRL_PTCL_ACC_BUF_EMPT;
	}
	if (!ocpp->msg_processed)
	{
		return CTRL_PTCL_PRC_BUF_FULL;
	}

	strcpy(ocpp->processive_buffer, ocpp->accumulative_buffer);
	ocpp->acc_buf_index = 0;
	ocpp->msg_received = false;
	ocpp->msg_processed = false;

	return CTRL_PTCL_OK;
}

Controller_Protocol_Result
_controller_ocpp_process_income
(
	Controller_OCPP *ocpp,
	Controller_TaskWrap *wrap
)
{
	ocpp->msg_processed = true;

	if (!_ocpp_parse_msg(ocpp))
	{
		return CTRL_PTCL_NON_VALID_MSG;
	}

	uprintf(DBUG_UART, 1000, 1000, "GOT `%s`\r", ocpp->processive_buffer);

	// IF MSG IS RESPONSE
	if (ocpp->message.type != CALL)
	{
		Controller_Protocol_Result res = _ocpp_append_resps(ocpp);
		#ifdef DEBUG
			if (res == CTRL_PTCL_OVER_RESP)
			uprintf(DBUG_UART, 100, 100, "RESPONSE PULL OVERWRITE\r");
		#endif
		return CTRL_PTCL_RESPONSE;
	}


	switch (ocpp->message.data.call.action)
	{
		case ACT_REMOTE_START_TRANSACTION:
			RSS_TASK_WRAP(wrap, ocpp->message.id);
			break;

		case ACT_REMOTE_STOP_TRANSACTION:
			RSTS_TASK_WRAP(wrap, ocpp->message.id);
			break;

		case ACT_CHANGE_AVAILABILITY:
			CA_TASK_WRAP(wrap, ocpp->message.id);
			break;

		case ACT_RESET:
			RT_TASK_WRAP(wrap, ocpp->message.id);
			break;

		case ACT_SEND_LOCAL_LIST:
			SLL_TASK_WRAP(wrap, ocpp->message.id);
			break;

		case ACT_GET_LOCAL_LIST_VERSION:
			GVL_TASK_WRAP(wrap, ocpp->message.id);
			break;

		case ACT_DATA_TRANSFER:
			DT_TASK_WRAP(wrap, ocpp->message.id);
			break;

		default: { return CTRL_PTCL_NO_SUCH_MSG; }
	}

	return CTRL_PTCL_OK;
}

Controller_Protocol_Result
_controller_ocpp_make_msg(Controller_OCPP *ocpp, OCPP_CallAction req, void *kwarg1, void *kwarg2, void *kwarg3)
{
	switch (req)
	{
		case ACT_BOOT_NOTIFICATION:
			ocpp_boot_notification_req(ocpp);
			break;

		case ACT_AUTHORIZE:
		 	ocpp_authorize_req(ocpp, (OCPP_IdTag *)kwarg1);
			break;

		case ACT_REMOTE_START_TRANSACTION:
			ocpp_remote_start_transaction_conf(ocpp, (bool *)kwarg1);
			break;

		case ACT_REMOTE_STOP_TRANSACTION:
			ocpp_remote_stop_transaction_conf(ocpp, (bool *)kwarg1);
			break;

		case ACT_START_TRANSACTION:
			ocpp_start_transaction_req(ocpp, (uint32_t *)kwarg1);
			break;

		case ACT_STOP_TRANSACTION:
			ocpp_stop_transaction_req(ocpp, (uint32_t *)kwarg1, (uint32_t *)kwarg2);
			break;

		case ACT_STATUS_NOTIFICATION:
			ocpp_status_notification_req(ocpp, (OCPP_ChargePointStatus *)kwarg1, (OCPP_ChargePointErrorCode *)kwarg2);
			break;

		case ACT_CHANGE_AVAILABILITY:
			ocpp_change_availabilty_conf(ocpp, (OCPP_AvailabilityStatus *)kwarg1);
			break;

		case ACT_RESET:
		 	ocpp_reset_conf(ocpp, (bool *)kwarg1);
			break;

		case ACT_SEND_LOCAL_LIST:
			ocpp_send_local_list_conf(ocpp, (OCPP_UpdateStatus *)kwarg1);
			break;

		case ACT_GET_LOCAL_LIST_VERSION:
			ocpp_get_local_list_version_conf(ocpp);
			break;

		case ACT_METER_VALUES:
			ocpp_meter_values_req(ocpp, (double *)kwarg1, (double *)kwarg2, (double *)kwarg3);
			break;

		case ACT_HEARTBEAT:
			ocpp_heartbeat_req(ocpp);
			break;

		case ACT_DATA_TRANSFER:
			ocpp_data_transfer_conf(ocpp, (OCPPDataTransferStatus *)kwarg1, (char *)kwarg2);
			break;

		default:;
			return CTRL_PTCL_NO_SUCH_MSG;
	}

	return CTRL_PTCL_OK;
}

Controller_Protocol_Result
_controller_ocpp_send_req(Controller_OCPP *ocpp, OCPP_CallAction req)
{
	if (req > ACT_AUTHORIZE)
	{
		return CTRL_PTCL_NO_SUCH_MSG;
	}

	char action_str[ACTION_LEN];
	strcpy(action_str, k_ACTIONS_STRINGS[req]);

	_ocpp_set_id_msg(ocpp);

	char request[OCPP_BUF_LEN];

	mjson_snprintf
	(
		request, OCPP_BUF_LEN,
		"[%u,%Q,%Q,%s]",
		CALL,
		ocpp->message.id,
		action_str,
		ocpp->message.data.call.payload
	);

	USART_Result res = uprintf(ocpp->uart, 100, OCPP_BUF_LEN+1, "%s\n", request);

	return (Controller_Protocol_Result)res;
}

Controller_Protocol_Result
_controller_ocpp_send_resp
(
	Controller_OCPP *ocpp,
	OCPP_MessageType type,
	OCPP_MessageID id
)
{
	if (type == CALLRESULT)
	{
		char req[OCPP_BUF_LEN];
		mjson_snprintf
		(
			req, OCPP_BUF_LEN,
			"[%u,%Q,%s]",
			ocpp->message.type,
			id,
			ocpp->message.data.call_result.payload
		);


		// SENDING
		uprintf(ocpp->uart, 100, OCPP_BUF_LEN, "%s\n", req);
		// SENDING
	}
	else if (type == CALLERROR)
	{

	}
	else
	{
		return CTRL_PTCL_NON_VALID_MSG;
	}

	return CTRL_PTCL_OK;
}

bool
_ocpp_parse_msg(Controller_OCPP *ocpp)
{
	if (!_ocpp_determine_message_type(ocpp))	{ return false; }
	if (!_ocpp_get_message_id(ocpp))			{ return false; }

	if (ocpp->message.type == CALL)
	{
		if (!_ocpp_get_action(ocpp))			{ return false; }
		if (!_ocpp_get_payload(ocpp, CALL))		{ return false; }
	}
	else if (ocpp->message.type == CALLRESULT)
	{
		if (!_ocpp_get_payload(ocpp, CALLRESULT)) { return false; }
	// 	// printf("NEW CALL RESULT REQ:\n");
	// 	// printf("\tID: `%ld`\n", ocpp->pres_msg.ID);
	// 	// printf("\tPAYLOAD: `%s`\n", ocpp->pres_msg.call_result.payload);
	}
	else if (ocpp->message.type == CALLERROR)
	{
		if (!_ocpp_get_call_error_code(ocpp)) { return false; }
		if (!_ocpp_get_call_error_descr(ocpp)) { return false; }
		if (!_ocpp_get_payload(ocpp, CALLERROR)) { return false; }
		// printf("NEW CALL ERROR REQ:\n");
		// printf("\tID: `%ld`\n", ocpp->pres_msg.ID);
		// printf("\tERROR CODE: `%d`\n", ocpp->pres_msg.call_error.error_code);
		// printf("\tERROR DESCRIPTION: `%s`\n", ocpp->pres_msg.call_error.error_dscr);
		// printf("\tERROR DETAILS: `%s`\n", ocpp->pres_msg.call_error.error_details);
	}

	return true;
}

bool
_ocpp_determine_message_type(Controller_OCPP *ocpp)
{
	double type = 0;
	int res = mjson_get_number(ocpp->processive_buffer, strlen(ocpp->processive_buffer), POS_MSG_TYPE, &type);
	if (res == 0 || !IS_MSG_TYPE((uint32_t)type))
		return false;

	ocpp->message.type = (OCPP_MessageType)type;
	// uprintf(ocpp->uart, 1000, 10, "type: %u\n", (OCPPMessageType)type);
	return true;
}

bool
_ocpp_get_message_id(Controller_OCPP *ocpp)
{
	char buf[ID_LEN] = {0};
	int res = mjson_get_string(ocpp->processive_buffer, strlen(ocpp->processive_buffer), POS_MSG_ID, buf, ID_LEN);
	if (res <= 0)
		return false;

	strcpy(ocpp->message.id, buf);
	// uprintf(ocpp->uart, 1000, 20+ID_LEN, "id: %s\n", buf);
	return true;
}

bool
_ocpp_get_action(Controller_OCPP *ocpp)
{
	char buf[ACTION_LEN];
	int res = mjson_get_string(ocpp->processive_buffer, strlen(ocpp->processive_buffer), POS_CL_ACT, buf, ACTION_LEN);
	if (res <= 0)
		return false;
	
	if (strcmp(buf, k_ACT_REMOTE_START_TRANSACTION) == 0)
		ocpp->message.data.call.action = ACT_REMOTE_START_TRANSACTION;
	else if (strcmp(buf, k_ACT_REMOTE_STOP_TRANSACTION) == 0)
		ocpp->message.data.call.action = ACT_REMOTE_STOP_TRANSACTION;
	else if (strcmp(buf, k_ACT_CHANGE_AVAILABILITY) == 0)
		ocpp->message.data.call.action = ACT_CHANGE_AVAILABILITY;
	else if (strcmp(buf, k_ACT_RESET) == 0)
		ocpp->message.data.call.action = ACT_RESET;
	else if (strcmp(buf, k_ACT_SEND_LOCAL_LIST) == 0)
		ocpp->message.data.call.action = ACT_SEND_LOCAL_LIST;
	else if (strcmp(buf, k_ACT_GET_LOCAL_LIST_VERSION) == 0)
		ocpp->message.data.call.action = ACT_GET_LOCAL_LIST_VERSION;
	else if (strcmp(buf, k_ACT_DATA_TRANSFER) == 0)
		ocpp->message.data.call.action = ACT_DATA_TRANSFER;
	else
		return false;
	

	return true;
}

bool
_ocpp_get_payload(Controller_OCPP *ocpp, OCPP_MessageType type)
{
	char path[5] = {0};
	if (type == CALL)
		strcpy(path, POS_CL_PAYLOAD);
	else if (type == CALLRESULT)
		strcpy(path, POS_CR_PAYLOAD);
	else if (type == CALLERROR)
		strcpy(path, POS_CE_ERR_DETL);
	else
		return false;


	const char *p;
    int n;
	if (mjson_find(ocpp->processive_buffer, strlen(ocpp->processive_buffer), path, &p, &n) != MJSON_TOK_OBJECT)
		return false;

	if (type == CALL)
	{
		strncpy(ocpp->message.data.call.payload, p, n);
		ocpp->message.data.call.payload[n] = 0;
	}
	else if (type == CALLRESULT)
	{
		strncpy(ocpp->message.data.call_result.payload, p, n);
		ocpp->message.data.call_result.payload[n] = 0;
	}
	else if (type == CALLERROR)
	{
		strncpy(ocpp->message.data.call_error.error_details, p, n);
		ocpp->message.data.call_error.error_details[n] = 0;
	}

	return true;
}

bool
_ocpp_get_call_error_code(Controller_OCPP *ocpp)
{
	char buf[ERR_CODE_LEN];
	int res = mjson_get_string(ocpp->processive_buffer, strlen(ocpp->processive_buffer), POS_CE_ERR_CODE, buf, ERR_CODE_LEN);
	if (res <= 0)
		return false;
	
	if (strcmp(buf, "GenericError") == 0)
		ocpp->message.data.call_error.error_code = ERC_GENERIC_ERROR;
	else
		return false;

	return true;
}

bool
_ocpp_get_call_error_descr(Controller_OCPP *ocpp)
{
	char buf[DSCR_LEN];
	int res = mjson_get_string(ocpp->processive_buffer, strlen(ocpp->processive_buffer), POS_CE_ERR_DSCR, buf, DSCR_LEN);
	if (res <= 0)
		return false;

	strcpy(ocpp->message.data.call_error.error_dscr, buf);

	return true;
}

void
_ocpp_set_id_msg(Controller_OCPP *ocpp)
{
	char id[ID_LEN];
	int_to_charset(ocpp->id_msg, id, 1);
	ocpp->id_msg++;
	strcpy(ocpp->message.id, id);
}

size_t
_ocpp_get_id_resp(Controller_OCPP *ocpp)
{
	uint32_t id = 0;
	charset_to_uint32(&id, ocpp->message.id);
	return (size_t)id;
}

Controller_Protocol_Result
_ocpp_append_resps(Controller_OCPP *ocpp)
{
	Controller_Protocol_Result res = CTRL_PTCL_OK;
	if (ocpp->q_resps == MAX_RESPONSES)
	{
		res = CTRL_PTCL_OVER_RESP;
		ocpp->q_resps = 0;
	}

	ocpp->responses[ocpp->q_resps++] = ocpp->message;
	if (ocpp->q_resps > 0)
	{
		ocpp->is_response = true;
	}

	return res;
}

void
_ocpp_delete_resp(Controller_OCPP *ocpp, size_t resp)
{
	for (size_t i = resp; i < ocpp->q_resps; ++i)
	{
		ocpp->responses[i] = ocpp->responses[i +1];
	}

	ocpp->q_resps -= 1;
}

bool
_ocpp_get_resp(Controller_OCPP *ocpp, size_t id)
{
	if (!ocpp->is_response)
	{
		return false;
	}

	for (size_t i = 0; i < ocpp->q_resps; ++i)
	{
		uint64_t p_id = 0;
		charset_to_uint64(&p_id, ocpp->responses[i].id);
		if (p_id == id)
		{
			ocpp->message = ocpp->responses[i];
			_ocpp_delete_resp(ocpp, i);
			return true;
		}
	}

	return false;
}
