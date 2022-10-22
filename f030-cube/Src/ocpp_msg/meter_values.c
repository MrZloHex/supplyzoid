#include "ocpp_msg/meter_values.h"
#include "rapi_msg/get_energy_usage.h"

#include "mjson.h"

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

	uint32_t ws;
	rapi_get_energy_usage_resp(rapi, &ws, NULL);
	uint32_t wh = ws / 3600;
	(void)wh;
	
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

	ocpp->pres_msg.type = CALL;
	ocpp->pres_msg.call.action = ACT_METER_VALUES;
	strcpy(ocpp->pres_msg.call.payload, payload);
}

void
ocpp_meter_values_conf
(
	OCPP *ocpp
)
{
	if (strcmp(ocpp->last_msg.ID, ocpp->pres_msg.ID) != 0)
		return;

	ocpp->_wait_resp = false;
}
