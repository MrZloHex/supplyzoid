#include "rapi_msg/enable.h"

#include "mjson.h"

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

	strcpy(rapi->buf_cmd, payload);
	rapi->buf_index = strlen(rapi->buf_cmd);
	rapi_append_chksum(rapi);
}

void
rapi_enable_resp(RAPI *rapi)
{

}

