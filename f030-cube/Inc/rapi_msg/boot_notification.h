#ifndef RAPI_BOOT_NOTIFICATION_H_
#define RAPI_BOOT_NOTIFICATION_H_

#include "controller.h"

Task_Result
rapi_boot_notification_req(Controller *ctrl);

void
rapi_boot_notification_resp(Controller_RAPI *rapi);

#endif /* RAPI_BOOTNOTIFICATION_H_ */