#include "rapi_msg/set_ammeter_settings.h"


void
rapi_set_ammeter_settings_req
(
	RAPI *rapi,
	u32 currentscalefactor,
	u32 currentoffset
)
{
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s %u %u^",
		"SA",
		currentscalefactor,
		currentoffset
	);

	strcpyy(rapi->buf_cmd, payload);
	rapi->buf_index = strlenn(rapi->buf_cmd);
	rapi_app_chksum(rapi);
}

void
rapi_set_ammeter_settings_resp(RAPI *rapi)
{

}

