#ifndef RAPI_SET_VOLTMETER_SETTINGS_H_
#define RAPI_SET_VOLTMETER_SETTINGS_H_

#include "RAPI.h"

void
rapi_set_voltmeter_settings_req
(
	RAPI *rapi,
	u32 voltscalefactor,
	u32 voltoffset
);

void
rapi_set_voltmeter_settings_resp(RAPI *rapi);

#endif /* RAPI_SET_VOLTMETER_SETTINGS_H_ */
