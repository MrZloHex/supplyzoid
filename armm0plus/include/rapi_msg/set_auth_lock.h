#ifndef RAPI_SET_AUTH_LOCK_H_
#define RAPI_SET_AUTH_LOCK_H_

#include "RAPI.h"


void
rapi_set_auth_lock_req
(
	RAPI *rapi,
	u8 lock_state
);

void
rapi_set_auth_lock_resp(RAPI *rapi);

#endif /* RAPI_SET_AUTH_LOCK_H_ */
