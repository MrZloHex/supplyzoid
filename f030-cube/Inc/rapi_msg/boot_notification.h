#ifndef RAPI_BOOT_NOTIFICATION_H_
#define RAPI_BOOT_NOTIFICATION_H_

#include "controller_rapi.h"

Controller_Task
rapi_boot_notification_req(Controller_RAPI *rapi);

void
rapi_boot_notification_resp(Controller_RAPI *rapi);

#endif /* RAPI_BOOTNOTIFICATION_H_ */