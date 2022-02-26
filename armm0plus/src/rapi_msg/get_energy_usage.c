#include "rapi_msg/get_energy_usage.h"

#include "convert.h"

void
rapi_get_energy_usage_req(RAPI *rapi)
{
    usart_rapi_println_str("Get energy state");
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s^",
		"GU"
	);

	strcpyy(rapi->buf_cmd, payload);
	rapi->buf_index = strlenn(rapi->buf_cmd);
	rapi_append_chksum(rapi);
}

void
rapi_get_energy_usage_resp
(
    RAPI *rapi,
    u32 *ws,
    u32 *whacc
)
{
    if (rapi->token_index < 3)
		return;

	if (ws != NULL)
		charset_to_u32(ws, rapi->tokens[1]);
	if (whacc != NULL)
		charset_to_u32(whacc, rapi->tokens[2]);
}