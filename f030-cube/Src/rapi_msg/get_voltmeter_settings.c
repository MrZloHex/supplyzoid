#include "rapi_msg/get_voltmeter_settings.h"

#include "mjson.h"

void
rapi_get_voltmeter_settings_req(RAPI *rapi)
{
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s^",
		"GM"
	);

	strcpy(rapi->buf_cmd, payload);
	rapi->buf_index = strlen(rapi->buf_cmd);
	rapi_append_chksum(rapi);
}

void
rapi_get_voltmeter_settings_resp
(
	RAPI *rapi,
	uint32_t *voltscalefactor,
	uint32_t *voltoffset
)
{
	if (rapi->token_index < 3)
		return;

	if (voltscalefactor != NULL)
		charset_to_uint32(voltscalefactor, rapi->tokens[1]);
	if (voltoffset!= NULL)
		charset_to_uint32(voltoffset, rapi->tokens[2]);
}
