#include "messages/meter_values.h"

void
ocpp_meter_values_req
(
	OCPP *ocpp,
	EVSE *evse
)
{
	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%d,%Q:%lu}",
		"connectorId",
		1,
		"meterValue",
		evse->meter_value
	);

	ocpp->now.type = CALL;
	ocpp->now.ID   = ocpp->id;
	ocpp->now.call.action = METER_VALUES;
	strcpyy(ocpp->now.call.payload, payload);
}

void
ocpp_meter_values_conf
(
	OCPP *ocpp
)
{
	if (ocpp->last.ID != ocpp->now.ID)
		return;

	ocpp->waiting_for_resp = false;
}
