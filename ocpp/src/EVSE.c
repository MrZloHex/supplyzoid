#include "EVSE.h"
#include "OCPP.h"

void
evse_init
(
	EVSE *evse
)
{
	evse->heartbeat_time = 0;
	evse->last_ping = time(NULL);
	evse->booted = false;

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
		if (now > evse->last_ping + evse->heartbeat_time)
			ocpp_send_req(ocpp, evse, BOOT_NOTIFICATION);

		evse->last_ping = now;
	}

	if (evse->booted || ocpp->waiting_for_resp)
	{
		ocpp_update(ocpp, evse);
	}
}
