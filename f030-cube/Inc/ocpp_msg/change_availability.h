#ifndef CHANGE_AVAILABILITY_H_
#define CHANGE_AVAILABILITY_H_

#include "OCPP.h"

typedef enum OCPP_AvailabilityStatus_E
{
	Accepted,
	Rejected,
	Scheduled
} OCPP_AvailabilityStatus;

typedef enum OCPP_AvailabilityType_E
{
	Inoperative,
	Operative
} OCPP_AvailabilityType;

void
ocpp_change_availability_req
(
	OCPP *ocpp
);

void
ocpp_change_availabilty_conf
(
	OCPP *ocpp,
    OCPP_AvailabilityStatus status
);

#endif /* CHANGE_AVAILABILTY_H_ */