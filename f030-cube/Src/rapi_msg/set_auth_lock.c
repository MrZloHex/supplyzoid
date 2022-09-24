#include "rapi_msg/set_auth_lock.h"

#include "mjson.h"

void
rapi_set_auth_lock_req
(
	RAPI *rapi,
	uint8_t lock_state
)
{
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s %u^",
		"S4",
		lock_state
	);

	strcpy(rapi->buf_cmd, payload);
	rapi->buf_index = strlen(rapi->buf_cmd);
	rapi_append_chksum(rapi);
}

void
rapi_set_auth_lock_resp(RAPI *rapi)
{

}
