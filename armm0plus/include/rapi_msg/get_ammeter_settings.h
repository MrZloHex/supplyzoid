#ifndef RAPI_GET_AMMETER_SETTINGS_H_
#define RAPI_GET_AMMETER_SETTINGS_H_

#include "RAPI.h"

void
rapi_get_ammeter_settings_req(RAPI *rapi);

void
rapi_get_ammeter_settings_resp
(
	RAPI *rapi,
	u32 *currentscalefactor,
	u32 *currentoffset
);

#endif /* RAPI_GET_AMMETER_SETTINGS_H_ */
