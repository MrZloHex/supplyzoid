#ifndef RAPI_GET_STATE_H_
#define RAPI_GET_STATE_H_

#include "RAPI.h"

void
rapi_get_state_req(RAPI *rapi);

void
rapi_get_state_resp
(
	RAPI *rapi,
	uint8_t *evse_state,
	uint32_t *elapsed_time,
	uint8_t *pilot_state,
	uint8_t * vflags
);

#endif /* RAPI_GET_STATE_H_ */