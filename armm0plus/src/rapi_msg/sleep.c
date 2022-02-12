#include "rapi_msg/sleep.h"

void
rapi_sleep_req(RAPI *rapi)
{
	usart_rapi_println_str("Sleep");
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s^",
		"FS"
	);

	strcpyy(rapi->buf_cmd, payload);
	rapi->buf_index = strlenn(rapi->buf_cmd);
	rapi_app_chksum(rapi);
}

void
rapi_sleep_resp(RAPI *rapi)
{

}

