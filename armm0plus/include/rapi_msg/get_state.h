#ifndef RAPI_GET_STATE_H_
#define RAPI_GET_STATE_H_

#include "RAPI.h"

void
rapi_get_state_req(RAPI *rapi);

void
rapi_get_state_resp
(
	RAPI *rapi,
	u8 *evse_state,
	u32 *elapsed_time,
	u8 *pilot_state,
	u8 * vflags
);

#endif /* RAPI_GET_STATE_H_ */