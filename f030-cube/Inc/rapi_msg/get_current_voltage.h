#ifndef RAPI_GET_CURRENT_VOLTAGE_H_
#define RAPI_GET_CURRENT_VOLTAGE_H_

#include "RAPI.h"

void
rapi_get_current_voltage_req(RAPI *rapi);

void
rapi_get_current_voltage_resp
(
	RAPI *rapi,
	uint64_t *milliamps,
	uint64_t *millivolts
);

#endif /* RAPI_GET_CURRENT_VOLTAGE_H_ */
