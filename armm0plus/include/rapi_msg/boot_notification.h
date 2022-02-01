#ifndef RAPI_BOOT_NOTIFICATION_H_
#define RAPI_BOOT_NOTIFICATION_H_

#include "RAPI.h"

void
rapi_boot_notification_req
(
    RAPI *rapi,
    OCPP *ocpp
);

void
rapi_boot_notification_resp(RAPI *rapi);

#endif /* RAPI_BOOTNOTIFICATION_H_ */