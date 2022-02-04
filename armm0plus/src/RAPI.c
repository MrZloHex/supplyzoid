#include "RAPI.h"
#include "usart_rapi.h"

#include "ocpp_msg/boot_notification.h"
#include "rapi_msg/boot_notification.h"
#include "rapi_msg/evse_state_transition.h"
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


bool
rapi_analyze(RAPI *rapi)
{

	usart_rapi_print_str("\nGet a new message: `");
	usart_rapi_print_str(rapi->buf_cmd);
	usart_rapi_print_str("`\n");

	rapi->tokens[0] = &rapi->buf_cmd[1];
	char *s = &rapi->buf_cmd[2];
	rapi->token_index = 1;
	u8 add_chksum = RAPI_SOC + rapi->buf_cmd[1];
	u8 xor_chksum = RAPI_SOC ^ rapi->buf_cmd[1];
	u8 hex_chksum;
	u8 chk_type = 0; // 0=none,1=additive,2=xor

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
			hex_str_to_u8(s, &hex_chksum);
			break;
		}
		else
		{
			add_chksum += *s;
			xor_chksum ^= *(s++);
		}
	}

	// usart_rapi_print_str("Hex: ");
	// char hex[3];
	// int_to_charset(hex_chksum, hex, 0);
	// usart_rapi_println_str(hex);
	// usart_rapi_print_str("XOR: ");
	// char add[3];
	// int_to_charset(xor_chksum, add, 0);
	// usart_rapi_println_str(add);

	bool ok = ((chk_type == 0) ||
				(chk_type == 1) && (hex_chksum == add_chksum) ||
				(chk_type == 2) && (hex_chksum == xor_chksum));

	if (!ok)
		rapi->token_index = 0;

	if (ok)
		usart_rapi_println_str("OK CHK SUM RAPi");
	else
		usart_rapi_println_str("ERR CHK SUM RAPi");

	return ok;
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


void
rapi_app_chksum(RAPI *rapi)
{
	u8 chksum = 0;
	for (size i = 0; i < rapi->buf_index-1; ++i)
		chksum ^= rapi->buf_cmd[i];

	u8_to_hex_str(rapi->buf_cmd + rapi->buf_index, chksum);
	rapi->buf_index += 2;
	rapi->buf_cmd[rapi->buf_index++] = RAPI_EOC;
	rapi->buf_cmd[rapi->buf_index] = 0;
}
