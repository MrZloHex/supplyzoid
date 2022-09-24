#include "rapi_msg/get_version.h"

#include "mjson.h"

void
rapi_get_version_req(RAPI *rapi)
{
	// usart_rapi_println_str("Get version");
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s^",
		"GV"
	);

	strcpy(rapi->buf_cmd, payload);
	rapi->buf_index = strlen(rapi->buf_cmd);
	rapi_append_chksum(rapi);
}

void
rapi_get_version_resp(RAPI *rapi)
{

}

