#ifndef RAPI_GET_STATE_H_
#define RAPI_GET_STATE_H_

#include "controller_rapi.h"

void
_rapi_get_state_req(Controller_RAPI *rapi);

void
_rapi_get_state_resp
(
	Controller_RAPI *rapi,
	uint8_t *evse_state,
	uint32_t *elapsed_time,
	uint8_t *pilot_state,
	uint8_t * vflags
);

#endif /* RAPI_GET_STATE_H_ */