#include "controller_ocpp.h"

#include "string.h"
#include "serial.h"

#include "ocpp_msg/boot_notification.h"

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

void
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

		default:
			// EXIT WITH ERROR
			return;
	}
}