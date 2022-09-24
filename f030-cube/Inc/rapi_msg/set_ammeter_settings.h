#ifndef RAPI_SET_AMMETER_SETTINGS_H_
#define RAPI_SET_AMMETER_SETTINGS_H_

#include "RAPI.h"

void
rapi_set_ammeter_settings_req
(
	RAPI *rapi,
	uint32_t currentscalefactor,
	uint32_t currentoffset
);

void
rapi_set_ammeter_settings_resp(RAPI *rapi);

#endif /* RAPI_SET_AMMETER_SETTINGS_H_ */
