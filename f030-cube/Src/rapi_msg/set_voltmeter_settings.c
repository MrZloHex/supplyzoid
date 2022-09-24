#include "rapi_msg/set_ammeter_settings.h"

#include "mjson.h"

void
rapi_set_voltmeter_settings_req
(
	RAPI *rapi,
	uint32_t voltscalefactor,
	uint32_t voltoffset
)
{
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s %u %u^",
		"SM",
		voltscalefactor,
		voltoffset
	);

	strcpy(rapi->buf_cmd, payload);
	rapi->buf_index = strlen(rapi->buf_cmd);
	rapi_append_chksum(rapi);

}

void
rapi_set_voltmeter_settings_resp(RAPI *rapi)
{

}
