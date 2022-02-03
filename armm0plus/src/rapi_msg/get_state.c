#include "rapi_msg/get_state.h"

#include "convert.h"

#define NULL ((void *)0)

void
rapi_get_state_req(RAPI *rapi)
{
    usart_rapi_println_str("Get state");
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s^",
		"GS"
	);

	strcpyy(rapi->buf_cmd, payload);
	rapi->buf_index = strlenn(rapi->buf_cmd);
	rapi_app_chksum(rapi);
}

void
rapi_get_state_resp
(
	RAPI *rapi,
	u8 *evse_state,
	u32 *elapsed_time,
	u8 *pilot_state,
	u8 * vflags
)
{
	if (rapi->token_index < 5)
		return;

	if (evse_state != NULL)
		charset_to_u8(evse_state, rapi->tokens[1]);
	if (elapsed_time != NULL)
		charset_to_u32(elapsed_time, rapi->tokens[2]);
	if (pilot_state != NULL)
		charset_to_u8(pilot_state, rapi->tokens[3]);
	if (vflags != NULL)
		charset_to_u8(vflags, rapi->tokens[4]);
}