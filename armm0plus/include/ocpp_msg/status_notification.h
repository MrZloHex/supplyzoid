#ifndef STATUS_NOTIFICATION_H_
#define STATUS_NOTIFICATION_H_

#include "OCPP.h"

// ChargePointErrorCode
#define NO_ERROR 0


void
ocpp_status_notification_req
(
    OCPP *ocpp,
    RAPI *rapi
);

void
ocpp_status_notification_conf
(
    OCPP *ocpp
);

#endif /* STATUS_NOTIFICATION_H_ */
