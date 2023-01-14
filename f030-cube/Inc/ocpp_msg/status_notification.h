#ifndef STATUS_NOTIFICATION_H_
#define STATUS_NOTIFICATION_H_

#include "controller_ocpp.h"

// ChargePointErrorCode
#define NO_ERROR 0


void
ocpp_status_notification_req
(
    Controller_OCPP *ocpp,
    OCPP_ChargePointStatus *state,
	OCPP_ChargePointErrorCode *error
);

void
ocpp_status_notification_conf
(
    Controller_OCPP *ocpp
);

#endif /* STATUS_NOTIFICATION_H_ */
