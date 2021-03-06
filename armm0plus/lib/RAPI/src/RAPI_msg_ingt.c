#include "RAPI_msg_ingt.h"

#include <convert.h>

bool
rapi_is_msg_correct(RAPI *rapi)
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
				chk_type = 2;

			*(s++) = '\0';
			hex_str_to_u8(&hex_chksum, s);
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

void
rapi_append_chksum(RAPI *rapi)
{
	u8 chksum = 0;
	for (size i = 0; i < rapi->buf_index-1; ++i)
		chksum ^= rapi->buf_cmd[i];

	u8_to_hex_str(chksum, rapi->buf_cmd + rapi->buf_index);
	rapi->buf_index += 2;
	rapi->buf_cmd[rapi->buf_index++] = RAPI_EOC;
	rapi->buf_cmd[rapi->buf_index] = 0;
}