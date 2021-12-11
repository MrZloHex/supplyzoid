#include "EVSE.h"
#include "OCPP.h"

#include "messages/boot_notification.h"

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
	if (evse->is_transaction)
		evse->meter_value++;

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

bool
evse_change_state
(
	EVSE *evse,
	EVSEState new_state
)
{
	if (evse->state == S_AVAILABLE)
	{
		if (new_state == S_AVAILABLE || new_state == S_FINISHING)
			return false;
		
		evse->state = new_state;
	}
	return true;
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

void
evse_stop_transaction
(
	EVSE *evse
)
{
	printf("Stop Transaction\n");
	evse->is_transaction = false;
}
