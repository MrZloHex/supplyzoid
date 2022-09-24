#include "ocpp_msg/heartbeat.h"

#include "mjson.h"

void
ocpp_heartbeat_req
(
	OCPP *ocpp
)
{
	char payload[PAYLOAD_LEN] = "{}";

	ocpp->pres_msg.type = CALL;
	ocpp->pres_msg.call.action = HEARTBEAT;
	strcpy(ocpp->pres_msg.call.payload, payload);
}

void
ocpp_heartbeat_conf
(
	OCPP *ocpp
)
{
	if (!strcmp(ocpp->last_msg.ID, ocpp->pres_msg.ID))
		return;

	ocpp->_wait_resp = false;
	
	size_t pay_len = strlen(ocpp->pres_msg.call_result.payload);

	char time[32];

	int res_int = mjson_get_string(ocpp->pres_msg.call_result.payload, pay_len, P_TIME, time, 32);
	if (res_int == -1)
		return;
}
