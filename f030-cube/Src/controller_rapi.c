#include "controller_rapi.h"

#include "string.h"

void
_controller_rapi_initialize
(
	Controller_RAPI *rapi,
	UART_HandleTypeDef *uart,
	TIM_HandleTypeDef *tim
)
{
	rapi->uart = uart;
	rapi->tim = tim;

	memset(rapi->accumulative_buffer, 0, RAPI_BUF_LEN);
	memset(rapi->processive_buffer,   0, RAPI_BUF_LEN);
	rapi->acc_buf_index = 0;
	rapi->prc_buf_index = 0;

	rapi->msg_received = false;
	rapi->msg_processed = true;
}

Controller_Protocol_Result
_controller_rapi_start_recv(Controller_RAPI *rapi)
{
	if (rapi->msg_received)
	{
		return CTRL_PTCL_ACC_BUF_FULL;
	}

	rapi->acc_buf_index = 0;
	HAL_StatusTypeDef res = HAL_UART_Receive_IT
							(
								rapi->uart,
								(uint8_t *)&rapi->accumulative_buffer[0],
								1
							);

	return (Controller_Protocol_Result)res;
}

Controller_Protocol_Result
_controller_rapi_transfer(Controller_RAPI *rapi)
{
	if (!rapi->msg_received)
	{
		return CTRL_PTCL_ACC_BUF_EMPT;
	}
	if (!rapi->msg_processed)
	{
		return CTRL_PTCL_PRC_BUF_FULL;
	}

	strcpy(rapi->processive_buffer, rapi->accumulative_buffer);
	rapi->msg_received = false;
	rapi->msg_processed = false;

	return CTRL_PTCL_OK;
}
