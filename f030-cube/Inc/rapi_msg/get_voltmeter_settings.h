#ifndef RAPI_GET_VOLTMETER_SETTINGS_H_
#define RAPI_GET_VOLTMETER_SETTINGS_H_

#include "RAPI.h"

void
rapi_get_voltmeter_settings_req(RAPI *rapi);

void
rapi_get_voltmeter_settings_resp
(
	RAPI *rapi,
	uint32_t *voltscalefactor,
	uint32_t *voltoffset
);

#endif /* RAPI_GET_VOLTMETER_SETTINGS_H_ */
