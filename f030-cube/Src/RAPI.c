#include "RAPI.h"
// #include "usart_rapi.h"

// #include "ocpp_msg/boot_notification.h"
#include "rapi_msg/boot_notification.h"
#include "rapi_msg/evse_state_transition.h"
#include "rapi_msg/ext_button.h"
// #include "OCPP.h"

#include "stdlib.h"
#include "string.h"
#include "serial.h"
#include "convert.h"


void
rapi_init
(
	RAPI *rapi,
	UART_HandleTypeDef *uart
)
{
	rapi->uart = uart;
	rapi->got_msg = false;
	rapi->proc_msg = true; 

    rapi->buffer = (char *)malloc(sizeof(char)*RAPI_BUF_LEN);
	rapi->buf_i = 0;
    rapi->buf_cmd = (char *)malloc(sizeof(char)*RAPI_BUF_LEN);
	rapi->buf_index = 0;
    rapi->tokens = (char **)malloc(sizeof(char *)*RAPI_MAX_TOKENS);
    rapi->token_index = 0;
}

void
rapi_reset(RAPI *rapi)
{
	memset(rapi->buf_cmd, 0, RAPI_BUF_LEN);
	memset(rapi->buffer, 0, RAPI_BUF_LEN);
	rapi->buf_index = 0;
	rapi->buf_i = 0;
}

void
rapi_deinit(RAPI *rapi)
{
	free(rapi->buf_cmd);
	free(rapi->tokens);
}


void
rapi_update
(
	RAPI *rapi,
	OCPP *ocpp
)
{
	if (rapi->got_msg && rapi->proc_msg)
	{
		strcpy(rapi->buf_cmd, rapi->buffer);
		rapi->got_msg = false;
		rapi->proc_msg = false;
	}

	if (!rapi->got_msg && HAL_UART_GetState(rapi->uart) == HAL_UART_STATE_READY)
	{
		HAL_UART_Receive_IT(rapi->uart, (uint8_t *)&rapi->buffer[0], 1);
		rapi->buf_i = 0;
	}

	if (!rapi->proc_msg)
	{
		// uprintf(rapi->uart, 10, 1024, "goT `%s`\r", rapi->buf_cmd);
		if (rapi_is_msg_correct(rapi))
			rapi_process_cmd(rapi, ocpp);
		rapi->proc_msg = true;
	}
}


void
rapi_process_cmd
(
	RAPI *rapi,
	OCPP *ocpp
)
{
	// uprintf(rapi->uart, 10, 1024, "goT `%s`\r", rapi->buf_cmd);
	// for (size_t i = 0; i < rapi->token_index; ++i)
	// {
	// 	uprintf(rapi->uart, 100, 64, "NEW TOKEN: `");
	// 	uprintf(rapi->uart, 100, 64, rapi->tokens[i]);
	// 	uprintf(rapi->uart, 100, 64, "`\r");
	// }

	char *cmd = rapi->tokens[0];
	switch (*cmd)
	{
		case 'A':
			switch (*(cmd+1))
			{
				case 'B':
					rapi_boot_notification_req(rapi, ocpp);
					break;
				case 'T':
					rapi_evse_state_transition_req(rapi, ocpp);
					break;
				case 'N':
					rapi_ext_button_req(rapi);
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

void
rapi_send_req(RAPI *rapi)
{
	uprintf(rapi->uart, 1000, RAPI_BUF_LEN, "%s", rapi->buf_cmd);
	rapi_reset(rapi);
}

bool
rapi_get_resp
(
	RAPI *rapi,
	OCPP *ocpp
)
{
	if (HAL_UART_AbortReceive(rapi->uart) != HAL_OK) { return false; }
	rapi_reset(rapi);

	while (1)
	{
		HAL_StatusTypeDef res = HAL_UART_Receive(rapi->uart, (uint8_t *)rapi->buf_cmd, 1, 100);
		if (res == HAL_ERROR)
			return false;
		else if (res == HAL_OK)
			if (*(rapi->buf_cmd) == '$')
				break;
	}

	if (uread(rapi->uart, 10, (rapi->buf_cmd)+1, RAPI_BUF_LEN-1, RAPI_EOC) == USART_IO_OK)
	{
		// uprintf(rapi->uart, 100, RAPI_BUF_LEN+4, "`%s`\r", rapi->buf_cmd);
		if (!rapi_is_msg_correct(rapi))
		{
			return false;
		}
	}

	rapi->buf_index = strlen(rapi->buf_cmd);
	

	if (rapi->buf_cmd[2] == 'K')
	{
		if (rapi->buf_cmd[1] == 'O')
			return true;
		else if (rapi->buf_cmd[1] == 'N')
			return false;
		else
			return false;
	}
	else
	{
		return false;
	}
}



bool
rapi_is_msg_correct(RAPI *rapi)
{
	rapi->tokens[0] = &rapi->buf_cmd[1];
	char *s = &rapi->buf_cmd[2];
	rapi->token_index = 1;
	uint8_t add_chksum = RAPI_SOC + rapi->buf_cmd[1];
	uint8_t xor_chksum = RAPI_SOC ^ rapi->buf_cmd[1];
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

void
rapi_append_chksum(RAPI *rapi)
{
	uint8_t chksum = 0;
	for (size_t i = 0; i < rapi->buf_index-1; ++i)
		chksum ^= rapi->buf_cmd[i];

	uint8_to_hex_str(chksum, rapi->buf_cmd + rapi->buf_index);
	rapi->buf_index += 2;
	rapi->buf_cmd[rapi->buf_index++] = RAPI_EOC;
	rapi->buf_cmd[rapi->buf_index] = 0;
}