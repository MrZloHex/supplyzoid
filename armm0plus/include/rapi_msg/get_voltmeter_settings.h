#ifndef RAPI_GET_VOLTMETER_SETTINGS_H_
#define RAPI_GET_VOLTMETER_SETTINGS_H_

#include "RAPI.h"

void
rapi_get_voltmeter_settings_req(RAPI *rapi);

void
rapi_get_voltmeter_settings_resp
(
	RAPI *rapi,
	u32 *voltscalefactor,
	u32 *voltoffset
);

#endif /* RAPI_GET_VOLTMETER_SETTINGS_H_ */
