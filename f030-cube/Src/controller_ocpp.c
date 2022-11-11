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

void
_controller_ocpp_process(Controller_OCPP *ocpp)
{
#ifdef DEBUG
	uprintf(ocpp->uart, 1000, 600, "GOT `%s`\n", ocpp->processive_buffer);
#endif

	ocpp->msg_processed = true;
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

	return (Controller_Protocol_Result)res;
}

void
_ocpp_set_id_msg(Controller_OCPP *ocpp)
{
	char id[ID_LEN];
	int_to_charset(ocpp->id_msg, id, 1);
	ocpp->id_msg++;
	strcpy(ocpp->message.id, id);
}
