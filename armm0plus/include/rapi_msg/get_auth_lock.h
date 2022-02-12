#ifndef RAPI_GET_AUTH_LOCK_H_
#define RAPI_GET_AUTH_LOCK_H_

#include "RAPI.h"

void
rapi_get_auth_lock_req(RAPI *rapi);

void
rapi_get_auth_lock_resp
(
	RAPI *rapi,
	u8 *lock_state
);

#endif /* RAPI_GET_AUTH_LOCK_H_ */
