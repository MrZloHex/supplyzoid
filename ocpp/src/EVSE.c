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
	OCPP *ocpp,
	EVSEState new_state
)
{
	if (evse->state == S_AVAILABLE)
	{
		if (new_state == S_AVAILABLE || new_state == S_FINISHING)
			return false;
	}
	else if (evse->state == S_PREPARING)
	{
		if (new_state == S_PREPARING || new_state == S_RESERVED || new_state == S_UNAVAILABLE)
			return false;
	}
	else if (evse->state == S_CHARGING)
	{
		if (new_state == S_PREPARING || new_state == S_CHARGING || new_state == S_RESERVED)
			return false;
	}
	else if (evse->state == S_SUSPENDED_EV)
	{
		if (new_state == S_PREPARING || new_state == S_SUSPENDED_EV || new_state == S_RESERVED)
			return false;
	}
	else if (evse->state == S_SUSPENDED_EVSE)
	{
		if (new_state == S_PREPARING || new_state == S_SUSPENDED_EVSE || new_state == S_RESERVED)
			return false;
	}
	else if (evse->state == S_FINISHING)
	{
		if (new_state > 2 && new_state < 8)
			return false;
	}
	else if (evse->state == S_RESERVED)
	{
		if (new_state > 2 && new_state < 8)
			return false;
	}
	else if (evse->state == S_UNAVAILABLE)
	{
		if (new_state > 5 && new_state < 9)
			return false;
	}
	else if (evse->state == S_FAULTED)
	{
		if (new_state == S_FAULTED)
			return false;
	}
	evse->state = new_state;
	return true;
}

char *evse_states_str[9] = {
	"Available",
	"Preparing",
	"Charging",
	"SuspendedEV",
	"SuspendedEVSE",
	"Finishing",
	"Reserved",
	"Unavailable",
	"Faulted"
};

char *
evse_get_state
(
	EVSE *evse
)
{
	return evse_states_str[evse->state -1];
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
