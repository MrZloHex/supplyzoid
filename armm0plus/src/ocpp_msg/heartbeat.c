#include "ocpp_msg/heartbeat.h"

void
ocpp_heartbeat_req
(
	OCPP *ocpp
)
{
	char payload[PAYLOAD_LEN] = "{}";

	ocpp->now.type = CALL;
	ocpp->now.ID   = ocpp->id;
	ocpp->now.call.action = HEARTBEAT;
	strcpyy(ocpp->now.call.payload, payload);
}

void
ocpp_heartbeat_conf
(
	OCPP *ocpp
)
{
	if (ocpp->last.ID != ocpp->now.ID)
		return;

	ocpp->waiting_for_resp = false;
	
	size pay_len = strlenn(ocpp->now.call_result.payload);

	char time[32];

	int res_int = mjson_get_string(ocpp->now.call_result.payload, pay_len, P_TIME, time, 32);
	if (res_int == -1)
		return;
}
