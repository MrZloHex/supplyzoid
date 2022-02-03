#include "ocpp_msg/meter_values.h"

void
ocpp_meter_values_req
(
	OCPP *ocpp,
	RAPI *rapi
)
{
	rapi_get_energy_usage_req(rapi);
	rapi_send_req(rapi);
	bool resp = rapi_get_resp(rapi, ocpp);
	if (!resp)
		return;

	u32 ws;
	rapi_get_energy_usage_resp(rapi, &ws, NULL);
	u32 wh = ws / 3600;

	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%d,%Q:%lu}",
		"connectorId",
		1,
		"meterValue",
		ws
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
