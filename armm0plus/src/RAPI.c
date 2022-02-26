#include "RAPI.h"
#include "usart_rapi.h"

#include "ocpp_msg/boot_notification.h"
#include "rapi_msg/boot_notification.h"
#include "rapi_msg/evse_state_transition.h"
#include "rapi_msg/ext_button.h"
#include "OCPP.h"

void
rapi_reset(RAPI *rapi)
{
	memsett(rapi->buf_cmd, 0, RAPI_BUF_LEN);
	rapi->buf_index = 0;
}

bool
rapi_update
(
	RAPI *rapi,
	OCPP *ocpp
)
{
	int buf_cnt = usart_rapi_available();
	if (buf_cnt)
	{
		for (size i = 0; i < buf_cnt; ++i)
		{
			char ch =  usart_rapi_read();
			if (ch == RAPI_SOC)
			{
				rapi->buf_cmd[0] = RAPI_SOC;
				rapi->buf_index = 1;
			}
			else if (rapi->buf_cmd[0] == RAPI_SOC)
			{
				if (rapi->buf_index < RAPI_BUF_LEN)
				{
					if (ch == RAPI_EOC)
					{
						if (rapi_analyze(rapi))
						{
							rapi_process_cmd(rapi, ocpp);
							return true;
						}
						else
						{
							rapi_reset(rapi);
							return false;
						}
					}
					else
					{
						rapi->buf_cmd[rapi->buf_index++] = ch;
					}
				}
				else
				{
					rapi_reset(rapi);
					return false;
				}
			}
		}
	}
	return false;
}


void
rapi_process_cmd
(
	RAPI *rapi,
	OCPP *ocpp
)
{
	// for (size i = 0; i < rapi->token_index; ++i)
	// {
	// 	usart_rapi_print_str("NEW TOKEN: `");
	// 	usart_rapi_print_str(rapi->tokens[i]);
	// 	usart_rapi_print_str("`\n");
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
					usart_rapi_println_str("ERROR: Unknown command");
			}
			break;
		case 'O':
		case 'N':
			break;
		default:
			usart_rapi_println_str("ERROR: Unknown command");
	}
}

void
rapi_send_req(RAPI *rapi)
{
	usart_rapi_print_str(rapi->buf_cmd);
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
