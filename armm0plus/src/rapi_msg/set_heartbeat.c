#include "rapi_msg/set_heartbeat.h"

void
rapi_set_heartbeat_req
(
	RAPI *rapi,
	my_time time
)
{	
	usart_rapi_println_str("Setting heartbeat");
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s %lu 0^",
		"SY",
		time
	);

	strcpyy(rapi->buf_cmd, payload);
	rapi->buf_index = strlenn(rapi->buf_cmd);
	rapi_append_chksum(rapi);
}

void
rapi_set_heartbeat_resp(RAPI *rapi)
{

}
