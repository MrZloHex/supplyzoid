#ifndef METER_VALUE_H_
#define METER_VALUE_H_

#include "OCPP.h"

void
ocpp_meter_values_req
(
	OCPP *ocpp,
	RAPI *rapi
);

void
ocpp_meter_values_conf
(
	OCPP *ocpp
);

#endif /* METER_VALUE_H_ */
