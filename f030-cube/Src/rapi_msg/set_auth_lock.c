#include "rapi_msg/set_auth_lock.h"

#include "mjson.h"

void
_rapi_set_auth_lock_req
(
	Controller_RAPI *rapi,
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

	strcpy(rapi->transmitter_buffer, payload);
	_rapi_append_chksum(rapi);
}

void
_rapi_set_auth_lock_resp(Controller_RAPI *rapi)
{

}
