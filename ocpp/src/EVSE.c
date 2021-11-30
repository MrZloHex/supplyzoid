#include "EVSE.h"
#include "OCPP.h"

#include "requests/boot_notification.h"

void
evse_init
(
	EVSE *evse
)
{
	evse->heartbeat_time = 0;
	evse->last_ping = time(NULL);
	evse->booted = false;

	evse->is_transaction = false;

	evse->state = S_AVAILABLE;
}

void
evse_update
(
	EVSE *evse,
	OCPP *ocpp
)
{
	if (!evse->booted && !ocpp->waiting_for_resp)
	{
		time_t now = time(NULL);
		if (now >= (evse->last_ping + evse->heartbeat_time))
		{
			ocpp_boot_notification_req(ocpp);
			ocpp_send_req(ocpp, BOOT_NOTIFICATION);
			evse->last_ping = time(NULL);
		}
	}

	if (evse->booted || ocpp->waiting_for_resp)
	{
		ocpp_update(ocpp, evse);
	}
}
