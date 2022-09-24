#include "rapi_msg/get_ammeter_settings.h"

#include "mjson.h"

void
rapi_get_ammeter_settings_req(RAPI *rapi)
{
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s^",
		"GA"
	);

	strcpy(rapi->buf_cmd, payload);
	rapi->buf_index = strlen(rapi->buf_cmd);
	rapi_append_chksum(rapi);
}

void
rapi_get_ammeter_settings_resp
(
	RAPI *rapi,
	uint32_t *currentscalefactor,
	uint32_t *currentoffset
)
{
	if (rapi->token_index < 3)
		return;

	if (currentscalefactor != NULL)
		charset_to_uint32(currentscalefactor, rapi->tokens[1]);
	if (currentoffset != NULL)
		charset_to_uint32(currentoffset, rapi->tokens[2]);
}

