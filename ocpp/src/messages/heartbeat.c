#include "messages/heartbeat.h"

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
	
}
