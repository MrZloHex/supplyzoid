#include "rapi_msg/enable.h"

void
rapi_enable_req(RAPI *rapi)
{
	// usart_rapi_println_str("Enable");
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s^",
		"FE"
	);

	strcpyy(rapi->buf_cmd, payload);
	rapi->buf_index = strlenn(rapi->buf_cmd);
	rapi_append_chksum(rapi);
}

void
rapi_enable_resp(RAPI *rapi)
{

}

