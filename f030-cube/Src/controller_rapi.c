#include "controller_rapi.h"

#include "string.h"
#include "serial.h"
#include "convert.h"

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
	rapi->token_index = 0;

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

void
_controller_rapi_process(Controller_RAPI *rapi)
{
#ifdef DEBUG
	uprintf(rapi->uart, 1000, 600, "GOT `%s`\r", rapi->processive_buffer);
#endif
	
	rapi->msg_processed = true;

	if (!_rapi_msg_validator(rapi))
	{
		// MAKE EXIT WITH ERROR
		return;
	}

	char *cmd = rapi->tokens[0];
	switch (*cmd)
	{
		case 'A':
			switch (*(cmd+1))
			{
				case 'B':
					// rapi_boot_notification_req(rapi, ocpp);
					break;
				case 'T':
					// rapi_evse_state_transition_req(rapi, ocpp);
					break;
				case 'N':
					// rapi_ext_button_req(rapi);
					break;
				default:
					uprintf(rapi->uart, 100, 25,"ERROR: Unknown command\r");
			}
			break;
		case 'O':
		case 'N':
			break;
		default:
			uprintf(rapi->uart, 100, 25,"ERROR: Unknown command\r");
	}
}



bool
_rapi_msg_validator(Controller_RAPI *rapi)
{
	rapi->tokens[0] = &rapi->processive_buffer[1];
	char *s = &rapi->processive_buffer[2];
	rapi->token_index = 1;
	uint8_t add_chksum = RAPI_SOC + rapi->processive_buffer[1];
	uint8_t xor_chksum = RAPI_SOC ^ rapi->processive_buffer[1];
	uint8_t hex_chksum;
	uint8_t chk_type = 0; // 0=none,1=additive,2=xor

	while (*s)
	{
		if (*s == ' ')
		{
			if (rapi->token_index >= RAPI_MAX_TOKENS)
			{
				chk_type = 255;
				break;
			}
			else
			{
				add_chksum += *s;
				xor_chksum ^= *s;
				*s = '\0';
				rapi->tokens[rapi->token_index++] = ++s;
			}
		}
		else if ((*s == '*') ||
				 (*s == '^'))
		{
			if (*s == '*')
				chk_type = 1;
			else if (*s == '^')
				chk_type = 2;

			*(s++) = '\0';
			hex_str_to_uint8(&hex_chksum, s);
			break;
		}
		else
		{
			add_chksum += *s;
			xor_chksum ^= *(s++);
		}
	}

	bool ok = ((chk_type == 0) ||
			   ((chk_type == 1) && (hex_chksum == add_chksum)) ||
			   ((chk_type == 2) && (hex_chksum == xor_chksum)));

	if (!ok)
		rapi->token_index = 0;


	return ok;
}