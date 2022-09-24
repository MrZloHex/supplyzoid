#include "rapi_msg/disable.h"

#include "mjson.h"

void
rapi_disable_req(RAPI *rapi)
{
	// usart_rapi_println_str("Disable");
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s^",
		"FD"
	);

	strcpy(rapi->buf_cmd, payload);
	rapi->buf_index = strlen(rapi->buf_cmd);
	rapi_append_chksum(rapi);
}

void
rapi_disable_resp(RAPI *rapi)
{

}

