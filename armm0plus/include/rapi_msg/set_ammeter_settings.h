#ifndef RAPI_SET_AMMETER_SETTINGS_H_
#define RAPI_SET_AMMETER_SETTINGS_H_

#include "RAPI.h"

void
rapi_set_ammeter_settings_req
(
	RAPI *rapi,
	u32 currentscalefactor,
	u32 currentoffset
);

void
rapi_set_ammeter_settings_resp(RAPI *rapi);

#endif /* RAPI_SET_AMMETER_SETTINGS_H_ */
