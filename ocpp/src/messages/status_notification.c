#include "messages/status_notification.h"

void
ocpp_status_notification_req
(
    OCPP *ocpp,
    EVSE *evse
)
{
	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%d,%Q:%Q,%Q:%Q,}",
		"connectorId",
		1,
		"errorCode",
		ChargePointErrorCode[NO_ERROR],
		"status",
		evse_get_state
	);

	ocpp->now.type = CALL;
	ocpp->now.ID   = ocpp->id;
	ocpp->now.call.action = STATUS_NOTIFICATION;
	strcpyy(ocpp->now.call.payload, payload);
}

void
ocpp_status_notification_conf
(
    OCPP *ocpp
)
{
    
}
