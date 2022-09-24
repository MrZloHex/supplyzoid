#include "rapi_msg/set_ammeter_settings.h"

#include "mjson.h"

void
rapi_set_ammeter_settings_req
(
	RAPI *rapi,
	uint32_t currentscalefactor,
	uint32_t currentoffset
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

	strcpy(rapi->buf_cmd, payload);
	rapi->buf_index = strlen(rapi->buf_cmd);
	rapi_append_chksum(rapi);
}

void
rapi_set_ammeter_settings_resp(RAPI *rapi)
{

}

