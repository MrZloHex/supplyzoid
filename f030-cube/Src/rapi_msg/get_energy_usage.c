#include "rapi_msg/get_energy_usage.h"

#include "mjson.h"

void
rapi_get_energy_usage_req(RAPI *rapi)
{
    // usart_rapi_println_str("Get energy state");
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s^",
		"GU"
	);

	strcpy(rapi->buf_cmd, payload);
	rapi->buf_index = strlen(rapi->buf_cmd);
	rapi_append_chksum(rapi);
}

void
rapi_get_energy_usage_resp
(
    RAPI *rapi,
    uint32_t *ws,
    uint32_t *whacc
)
{
    if (rapi->token_index < 3)
		return;

	if (ws != NULL)
		charset_to_uint32(ws, rapi->tokens[1]);
	if (whacc != NULL)
		charset_to_uint32(whacc, rapi->tokens[2]);
}
