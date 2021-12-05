#ifndef STATUS_NOTIFICATION_H_
#define STATUS_NOTIFICATION_H_

#include "OCPP.h"

void
ocpp_status_notification_req
(
    OCPP *ocpp
);

void
ocpp_status_notification_conf
(
    OCPP *ocpp
);

#endif /* STATUS_NOTIFICATION_H_ */