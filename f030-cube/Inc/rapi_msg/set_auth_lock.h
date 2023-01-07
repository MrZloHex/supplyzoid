#ifndef RAPI_SET_AUTH_LOCK_H_
#define RAPI_SET_AUTH_LOCK_H_

#include "controller_rapi.h"


void
_rapi_set_auth_lock_req
(
	Controller_RAPI *rapi,
	uint8_t lock_state
);

void
rapi_set_auth_lock_resp(Controller_RAPI *rapi);

#endif /* RAPI_SET_AUTH_LOCK_H_ */
