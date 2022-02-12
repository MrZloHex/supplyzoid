#include "rapi_msg/set_auth_lock.h"

void
rapi_set_auth_lock_req
(
	RAPI *rapi,
	u8 lock_state
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

	strcpyy(rapi->buf_cmd, payload);
	rapi->buf_index = strlenn(rapi->buf_cmd);
	rapi_app_chksum(rapi);
}

void
rapi_set_auth_lock_resp(RAPI *rapi)
{

}
