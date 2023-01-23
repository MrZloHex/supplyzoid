#ifndef CHANGE_AVAILABILITY_H_
#define CHANGE_AVAILABILITY_H_

#include "controller_ocpp.h"

void
ocpp_change_availability_req
(
	Controller_OCPP *ocpp
);

void
ocpp_change_availabilty_conf
(
	Controller_OCPP *ocpp,
    OCPP_AvailabilityStatus *status
);

#endif /* CHANGE_AVAILABILTY_H_ */