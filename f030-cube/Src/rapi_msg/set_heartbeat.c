#include "rapi_msg/set_heartbeat.h"

#include "mjson.h"

void
rapi_set_heartbeat_req
(
	RAPI *rapi,
	uint64_t time
)
{	
	// usart_rapi_println_str("Setting heartbeat");
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s %lu 0^",
		"SY",
		time
	);

	strcpy(rapi->buf_cmd, payload);
	rapi->buf_index = strlen(rapi->buf_cmd);
	rapi_append_chksum(rapi);
}

void
rapi_set_heartbeat_resp(RAPI *rapi)
{

}
