#include "rapi_msg/get_current_voltage.h"

#include "mjson.h"

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

	strcpy(rapi->buf_cmd, payload);
	rapi->buf_index = strlen(rapi->buf_cmd);
	rapi_append_chksum(rapi);
}

void
rapi_get_current_voltage_resp
(
	RAPI *rapi,
	uint64_t *milliamps,
	uint64_t *millivolts
)
{
	if (rapi->token_index < 3)
		return;

	if (milliamps != NULL)
		charset_to_uint64(milliamps, rapi->tokens[1]);
	if (millivolts != NULL)
		charset_to_uint64(millivolts, rapi->tokens[2]);
}

