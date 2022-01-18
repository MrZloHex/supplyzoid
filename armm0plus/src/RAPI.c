#include "RAPI.h"
#include "usart_rapi.h"

void
rapi_reset(RAPI *rapi)
{
	memsett(rapi->buf_cmd, 0, RAPI_BUF_LEN);
	rapi->buf_index = 0;
}

void
rapi_update(RAPI *rapi)
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

			if (rapi->buf_cmd[0] == RAPI_SOC)
			{
				if (rapi->buf_index < RAPI_BUF_LEN)
				{
					if (ch == RAPI_EOC)
					{
						if (rapi_analyze(rapi))
						{
							// process
						}
						else
						{
							rapi_reset(rapi);
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
				}
			}
		}
	}
}


bool
rapi_analyze(RAPI *rapi)
{
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
				chk_type == 2;

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

	bool ok = ((chk_type == 0) ||
				(chk_type == 1) && (hex_chksum == add_chksum) ||
				(chk_type == 2) && (hex_chksum == xor_chksum));

	if (!ok)
		rapi->token_index = 0;

	return ok;
}
