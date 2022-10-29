#include "ocpp_msg/meter_values.h"
#include "rapi_msg/get_energy_usage.h"
#include "rapi_msg/get_current_voltage.h"

#include "mjson.h"
#include "serial.h"
#include "time.h"

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

	rapi_get_current_voltage_req(rapi);
	rapi_send_req(rapi);
	resp = rapi_get_resp(rapi, ocpp);
	if (!resp)
		return;

	uint64_t ma;
	uint64_t mv;
	rapi_get_current_voltage_resp(rapi, &ma, &mv);
	uint32_t a = ma /1000;
	uint32_t v = mv /1000;
	uint32_t w = a*v;


	char time[25] = {0};
	get_rtc_time(ocpp->rtc, time);
	

	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%d,%Q:[{%Q:%Q,%Q:[{%Q:\"%u\",%Q:%Q},{%Q:\"%u\",%Q:%Q},{%Q:\"%u\",%Q:%Q},{%Q:\"%u\",%Q:%Q}]}]}",
		"connectorId",
		1,
		"meterValue",
		"timestamp",
		time,
		"sampledValue",
		"value", wh,
		"unit", "Wh",
		"value", w,
		"unit", "W",
		"value", a,
		"unit", "A",
		"value", v,
		"unit", "V"
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
