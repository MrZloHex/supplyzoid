#ifndef BOOT_NOTIFICATION_H_
#define BOOT_NOTIFICATION_H_

#include "controller_ocpp.h"

#define P_INTERVAL     "$.interval"
#define P_STATUS       "$.status"
#define P_CURRENT_TIME "$.currentTime"

void
ocpp_boot_notification_req(Controller_OCPP *ocpp);

void
ocpp_boot_notification_conf(Controller_OCPP *ocpp);

#endif /* BOOT_NOTIFICATION_H_ */
