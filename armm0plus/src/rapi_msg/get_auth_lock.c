#include "rapi_msg/get_auth_lock.h"

void
rapi_get_auth_lock_req(RAPI *rapi)
{
	char payload[RAPI_BUF_LEN];
	mjson_snprintf
	(
		payload, RAPI_BUF_LEN,
		"$%s^",
		"G4"
	);

	strcpyy(rapi->buf_cmd, payload);
	rapi->buf_index = strlenn(rapi->buf_cmd);
	rapi_app_chksum(rapi);
}

void
rapi_get_auth_lock_resp
(
	RAPI *rapi,
	u8 *lock_state
)
{
	if (rapi->token_index < 2)
		return;

	if (lock_state != NULL)
		charset_to_u8(lock_state, rapi->tokens[1]);
}

