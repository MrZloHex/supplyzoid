#include "controller_ocpp.h"

#include "string.h"
#include "serial.h"
#include "mjson.h"
#include "convert.h"

#include "ocpp_msg/boot_notification.h"

const static char	k_ACT_BOOT_NOTIFICATION[]		 = "BootNotification";
const static char	k_ACT_START_TRANSACTION[]        = "StartTransaction";
const static char	k_ACT_STOP_TRANSACTION[]         = "StopTransaction";
const static char	k_ACT_STATUS_NOTIFICATION[]      = "StatusNotification";
const static char	k_ACT_METER_VALUES[]             = "MeterValues";
const static char	k_ACT_HEARTBEAT[]                = "HeartBeat";
const static char	k_ACT_DATA_TRANSFER[]            = "DataTransfer";
const static char	k_ACT_AUTHORIZE[]                = "Authorize";
const static char  *k_ACTIONS_STRINGS[8] =
{
	k_ACT_BOOT_NOTIFICATION, k_ACT_START_TRANSACTION, k_ACT_STOP_TRANSACTION,
	k_ACT_STATUS_NOTIFICATION, k_ACT_METER_VALUES, k_ACT_HEARTBEAT,
	k_ACT_DATA_TRANSFER, k_ACT_AUTHORIZE
};


void
_controller_ocpp_initialize
(
	Controller_OCPP *ocpp,
	UART_HandleTypeDef *uart,
	TIM_HandleTypeDef *tim,
	RTC_HandleTypeDef *rtc
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
}

Controller_Protocol_Result
_controller_ocpp_start_recv(Controller_OCPP *ocpp)
{
	if (ocpp->msg_received)
	{
		return CTRL_PTCL_ACC_BUF_FULL;
	}

	ocpp->acc_buf_index = 0;
	HAL_StatusTypeDef res = HAL_UART_Receive_IT
							(
								ocpp->uart,
								(uint8_t *)&ocpp->accumulative_buffer[0],
								1
							);

	return (Controller_Protocol_Result)res;
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
	ocpp->msg_received = false;
	ocpp->msg_processed = false;

	return CTRL_PTCL_OK;
}

Controller_TaskResult
_controller_ocpp_process(Controller_OCPP *ocpp)
{
#ifdef DEBUG
	uprintf(ocpp->uart, 1000, 600, "GOT `%s`\n", ocpp->processive_buffer);
#endif

	ocpp->msg_processed = true;
	Controller_Task task = { .type = NO_TASK };

	_ocpp_parce_msg(ocpp);

	CONTROLLER_TASK_RESULT(task);
}

Controller_TaskResult
_controller_ocpp_make_req(Controller_OCPP *ocpp, Task_OCPP_MakeReq req)
{
	switch (req.action)
	{
		case ACT_BOOT_NOTIFICATION:
			ocpp_boot_notification_req(ocpp);
			break;

		case ACT_START_TRANSACTION:
			break;

		case ACT_STOP_TRANSACTION:
			break;

		case ACT_STATUS_NOTIFICATION:
			break;

		case ACT_METER_VALUES:
			break;

		case ACT_HEARTBEAT:
			break;

		case ACT_AUTHORIZE:
			break;

		default:;
			CONTROLLER_TASK_OCPP_ERROR(CTRL_PTCL_NO_SUCH_MSG);
	}

	Controller_Task task = { .type = TASK_OCPP_SEND_REQ, .data.ocpp_send_req.action = req.action };
	CONTROLLER_TASK_RESULT(task);
}

Controller_Protocol_Result
_controller_ocpp_send_req(Controller_OCPP *ocpp, Task_OCPP_SendReq req)
{
	if (req.action > ACT_AUTHORIZE)
	{
		return CTRL_PTCL_NO_SUCH_MSG;
	}

	char action_str[ACTION_LEN];
	strcpy(action_str, k_ACTIONS_STRINGS[req.action]);

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

	USART_Result res = uprintf(ocpp->uart, 1000, OCPP_BUF_LEN+1, "%s\n", request);

	_ocpp_add_expected_msg(ocpp, req.action);

	return (Controller_Protocol_Result)res;
}



bool
_ocpp_parce_msg(Controller_OCPP *ocpp)
{
	if (!_ocpp_determine_message_type(ocpp))	{ return false; }
	if (!_ocpp_get_message_id(ocpp))			{ return false; }

	if (ocpp->message.type == CALL)
	{
		if (!_ocpp_get_action(ocpp))			{ return false; }
		// if (!_ocpp_get_payload(ocpp, CALL))		{ return false; }
	}
	else if (ocpp->message.type == CALLRESULT)
	{
	// 	if (ocpp_get_payload(ocpp, CALLRESULT) == RES_ERROR) { return RES_ERROR; }
	// 	// printf("NEW CALL RESULT REQ:\n");
	// 	// printf("\tID: `%ld`\n", ocpp->pres_msg.ID);
	// 	// printf("\tPAYLOAD: `%s`\n", ocpp->pres_msg.call_result.payload);
	// }
	// else if (ocpp->message.type == CALLERROR)
	// {
	// 	if (ocpp_get_call_error_code(ocpp) == RES_ERROR) { return RES_ERROR; }
	// 	if (ocpp_get_call_error_descr(ocpp) == RES_ERROR) {return RES_ERROR; }
		// if (ocpp_get_payload(ocpp, CALLERROR) == RES_ERROR) { return RES_ERROR; }
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
	
	if (strcmp(buf, "RemoteStartTransaction") == 0)
		ocpp->message.data.call.action = ACT_REMOTE_START_TRANSACTION;
	else if (strcmp(buf, "RemoteStopTransaction") == 0)
		ocpp->message.data.call.action = ACT_REMOTE_STOP_TRANSACTION;
	else if (strcmp(buf, "ChangeAvailability") == 0)
		ocpp->message.data.call.action = ACT_CHANGE_AVAILABILITY;
	else if (strcmp(buf, "Reset") == 0)
		ocpp->message.data.call.action = ACT_RESET;
	else
		return false;
	

	return true;
}

bool
ocpp_get_payload(Controller_OCPP *ocpp, OCPP_MessageType type)
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
		strncpy(ocpp->message.data.call.payload, p, n);
	else if (type == CALLRESULT)
		strncpy(ocpp->message.data.call_result.payload, p, n);
	else if (type == CALLERROR)
		strncpy(ocpp->message.data.call_error.error_details, p, n);

	return true;
}

void
_ocpp_add_expected_msg(Controller_OCPP *ocpp, OCPP_CallAction action)
{
	if (ocpp->q_ex_msg == MAX_EXPECTED_MSG)
	{
		// exit with error code!
		return;
	}

	OCPP_Expected_Message ex_msg = { .call_action = action }; 
	strcpy(ex_msg.id, ocpp->message.id);

	ocpp->expected_msgs[ocpp->q_ex_msg++] = ex_msg;
}

void
_ocpp_set_id_msg(Controller_OCPP *ocpp)
{
	char id[ID_LEN];
	int_to_charset(ocpp->id_msg, id, 1);
	ocpp->id_msg++;
	strcpy(ocpp->message.id, id);
}
