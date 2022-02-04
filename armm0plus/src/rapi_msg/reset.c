#include "rapi_msg/reset.h"

void
rapi_reset_req(RAPI *rapi)
{
    usart_rapi_println_str("Reset");
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s^",
		"GR"
	);

	strcpyy(rapi->buf_cmd, payload);
	rapi->buf_index = strlenn(rapi->buf_cmd);
	rapi_app_chksum(rapi);
}

void
rapi_reset_resp(RAPI *rapi)
{

}