#include "RAPI.h"
// #include "usart_rapi.h"

// #include "ocpp_msg/boot_notification.h"
// #include "rapi_msg/boot_notification.h"
// #include "rapi_msg/evse_state_transition.h"
// #include "rapi_msg/ext_button.h"
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
    rapi->buf_cmd = (char *)malloc(sizeof(char)*RAPI_BUF_LEN);
	rapi->buf_index = 0;
    rapi->tokens = (char **)malloc(sizeof(char *)*RAPI_MAX_TOKENS);
    rapi->token_index = 0;
}

void
rapi_reset(RAPI *rapi)
{
	memset(rapi->buf_cmd, 0, RAPI_BUF_LEN);
	rapi->buf_index = 0;
}

void
rapi_deinit(RAPI *rapi)
{
	free(rapi->buf_cmd);
	free(rapi->tokens);
}


bool
rapi_update
(
	RAPI *rapi,
	OCPP *ocpp
)
{
	char buffer[RAPI_BUF_LEN] = {0};

	USART_Result state = uread(rapi->uart, 100, buffer, 100, '\r');
	
	uprintf(rapi->uart, 100, 6, "S %d\r", state);

	if (state == USART_IO_OK) uprintf(rapi->uart, 1000, 110, "`%s`\r", buffer);

	if (state == USART_IO_ERROR) Error_Handler();

	return false;
}


void
rapi_process_cmd
(
	RAPI *rapi,
	OCPP *ocpp
)
{
	for (size_t i = 0; i < rapi->token_index; ++i)
	{
		uprintf(rapi->uart, 100, 64, "NEW TOKEN: `");
		uprintf(rapi->uart, 100, 64, rapi->tokens[i]);
		uprintf(rapi->uart, 100, 64, "`\r");
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

void
rapi_send_req(RAPI *rapi)
{
	uprintf(rapi->uart, 1000, RAPI_BUF_LEN, "%s\r", rapi->buf_cmd);
	rapi_reset(rapi);
}

bool
rapi_get_resp
(
	RAPI *rapi,
	OCPP *ocpp
)
{
	rapi_reset(rapi);
	bool upd = false;
	do
		upd = rapi_update(rapi, ocpp);
	while (!upd);
	

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