#ifndef RAPI_GET_AMMETER_SETTINGS_H_
#define RAPI_GET_AMMETER_SETTINGS_H_

#include "RAPI.h"

void
rapi_get_ammeter_settings_req(RAPI *rapi);

void
rapi_get_ammeter_settings_resp
(
	RAPI *rapi,
	uint32_t *currentscalefactor,
	uint32_t *currentoffset
);

#endif /* RAPI_GET_AMMETER_SETTINGS_H_ */
