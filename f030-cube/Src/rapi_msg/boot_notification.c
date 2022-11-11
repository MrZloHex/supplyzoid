#include "rapi_msg/boot_notification.h"
#include "serial.h"

#include "ocpp_types.h"

Controller_Task
rapi_boot_notification_req(Controller_RAPI *rapi)
{
	uprintf(rapi->uart, 1000, 100, "BOOT NOTIFICAZTION\r");
	// handle BOOT TROUBLES
	Controller_Task task = { .type = TASK_OCPP_MAKE_REQ, .data = { .ocpp_make_req = { ACT_BOOT_NOTIFICATION }}};
	return task;
}

void
rapi_boot_notification_resp(Controller_RAPI *rapi)
{

}
