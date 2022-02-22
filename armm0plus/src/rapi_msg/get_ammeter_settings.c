#include "rapi_msg/get_ammeter_settings.h"

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

	strcpyy(rapi->buf_cmd, payload);
	rapi->buf_index = strlenn(rapi->buf_cmd);
	rapi_app_chksum(rapi);
}

void
rapi_get_ammeter_settings_resp
(
	RAPI *rapi,
	u32 *currentscalefactor,
	u32 *currentoffset
)
{
	if (rapi->token_index < 3)
		return;

	if (currentscalefactor != NULL)
		charset_to_u32(currentscalefactor, rapi->tokens[1]);
	if (currentoffset != NULL)
		charset_to_u32(currentoffset, rapi->tokens[2]);
}

