#include "requests/boot_notification.h"

void
ocpp_boot_notification_conf
(
	OCPP *ocpp,
	EVSE *evse
)
{
	if (ocpp->last.ID != ocpp->now.ID)
		return;

	
	printf("HANDLING BOOT NOTIFICATIOSN CONFIRMATION\n");

	ocpp->waiting_for_resp = false;
}