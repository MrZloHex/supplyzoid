#include "rapi_msg/set_ammeter_settings.h"

void
rapi_set_voltmeter_settings_req
(
	RAPI *rapi,
	u32 voltscalefactor,
	u32 voltoffset
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

	strcpyy(rapi->buf_cmd, payload);
	rapi->buf_index = strlenn(rapi->buf_cmd);
	rapi_append_chksum(rapi);

}

void
rapi_set_voltmeter_settings_resp(RAPI *rapi)
{

}
