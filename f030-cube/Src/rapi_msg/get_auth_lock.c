#include "rapi_msg/get_auth_lock.h"

#include "mjson.h"

void
_rapi_get_auth_lock_req(Controller_RAPI *rapi)
{
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s^",
		"G4"
	);

	strcpy(rapi->transmitter_buffer, payload);
	_rapi_append_chksum(rapi);
}

void
_rapi_get_auth_lock_resp
(
	Controller_RAPI *rapi,
	uint8_t *lock_state
)
{
	if (rapi->token_index < 2)
		return;

	if (lock_state != NULL)
		charset_to_uint8(lock_state, rapi->tokens[1]);
}

