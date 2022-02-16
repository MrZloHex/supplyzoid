#include "rapi_msg/get_current_voltage.h"

#include "convert.h"

void
rapi_get_current_voltage_req(RAPI *rapi)
{
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s^",
		"GG"
	);

	strcpyy(rapi->buf_cmd, payload);
	rapi->buf_index = strlenn(rapi->buf_cmd);
	rapi_app_chksum(rapi);
}

void
rapi_get_current_voltage_resp
(
	RAPI *rapi,
	u64 *milliamps,
	u64 *millivolts
)
{
	if (rapi->token_index < 3)
		return;

	if (milliamps != NULL)
		charset_to_u64(milliamps, rapi->tokens[1]);
	if (millivolts != NULL)
		charset_to_u64(millivolts, rapi->tokens[2]);
}

