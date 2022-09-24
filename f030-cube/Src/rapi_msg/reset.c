#include "rapi_msg/reset.h"

#include "mjson.h"

void
rapi_reset_req(RAPI *rapi)
{
	// usart_rapi_println_str("Reset");
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s^",
		"FR"
	);

	strcpy(rapi->buf_cmd, payload);
	rapi->buf_index = strlen(rapi->buf_cmd);
	rapi_append_chksum(rapi);
}

void
rapi_reset_resp(RAPI *rapi)
{

}

