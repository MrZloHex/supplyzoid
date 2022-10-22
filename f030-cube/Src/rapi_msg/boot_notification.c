#include "rapi_msg/boot_notification.h"

#include "ocpp_msg/boot_notification.h"
#include "OCPP.h"

void
rapi_boot_notification_req
(
    RAPI *rapi,
    OCPP *ocpp
)
{
	// handle BOOT TROUBLES
	ocpp_boot_notification_req(ocpp);
	ocpp_send_req(ocpp, ACT_BOOT_NOTIFICATION);

}

void
rapi_boot_notification_resp(RAPI *rapi)
{

}
