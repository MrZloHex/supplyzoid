#ifndef RAPI_SET_HEARTBEAT_H_
#define RAPI_SET_HEARTBEAT_H_

#include "RAPI.h"
#include "time_format.h"

void
rapi_set_heartbeat_req
(
	RAPI *rapi,
	my_time time
);

void
rapi_set_heartbeat_resp(RAPI *rapi);

#endif /* RAPI_SET_HEARTBEAT_H_ */