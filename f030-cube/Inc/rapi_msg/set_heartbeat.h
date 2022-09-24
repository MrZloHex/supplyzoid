#ifndef RAPI_SET_HEARTBEAT_H_
#define RAPI_SET_HEARTBEAT_H_

#include "RAPI.h"

void
rapi_set_heartbeat_req
(
	RAPI *rapi,
	uint64_t time
);

void
rapi_set_heartbeat_resp(RAPI *rapi);

#endif /* RAPI_SET_HEARTBEAT_H_ */