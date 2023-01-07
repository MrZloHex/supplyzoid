#ifndef RAPI_GET_AUTH_LOCK_H_
#define RAPI_GET_AUTH_LOCK_H_

#include "controller_rapi.h"

void
_rapi_get_auth_lock_req(Controller_RAPI *rapi);

void
_rapi_get_auth_lock_resp
(
	Controller_RAPI *rapi,
	uint8_t *lock_state
);

#endif /* RAPI_GET_AUTH_LOCK_H_ */
