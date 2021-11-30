#include "EVSE.h"
#include "OCPP.h"

#include "requests/boot_notification.h"

void
evse_init
(
	EVSE *evse
)
{
	evse->HeartbeatInterval = 0;
	evse->last_ping = time(NULL);
	evse->booted = false;

	evse->is_transaction = false;
	evse->meter_value = 0;

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
		if (now >= (evse->last_ping + evse->HeartbeatInterval))
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

void
evse_start_transaction
(
	EVSE *evse
)
{
	printf("Start Transaction\n");
	evse->is_transaction = true;
}
