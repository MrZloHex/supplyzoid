#include "rapi_msg/get_version.h"

void
rapi_get_version_req(RAPI *rapi)
{
	usart_rapi_println_str("Get version");
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s^",
		"GV"
	);

	strcpyy(rapi->buf_cmd, payload);
	rapi->buf_index = strlenn(rapi->buf_cmd);
	rapi_app_chksum(rapi);
}

void
rapi_get_version_resp(RAPI *rapi)
{

}

