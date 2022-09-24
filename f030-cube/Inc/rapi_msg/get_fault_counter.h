#ifndef RAPI_GET_FAULT_COUNTER_H_
#define RAPI_GET_FAULT_COUNTER_H_

#include "RAPI.h"

void
rapi_get_fault_counter_req(RAPI *rapi);

void
rapi_get_fault_counter_resp
(
	RAPI *rapi,
	uint8_t *gfitripcnt,
	uint8_t *nogndtripcnt,
	uint8_t *stuckrelaytripcnt
);

#endif /* RAPI_GET_FAULT_COUNTER_H_ */
