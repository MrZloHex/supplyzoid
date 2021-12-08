#include "messages/status_notification.h"

void
ocpp_status_notification_req
(
    OCPP *ocpp
)
{
	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%Q,%Q:%Q}",
		"connectorId",
		VENDOR,
		"chargePointModel",
		MODEL
	);

	ocpp->now.type = CALL;
	ocpp->now.ID   = ocpp->id;
	ocpp->now.call.action = BOOT_NOTIFICATION;
	strcpyy(ocpp->now.call.payload, payload);
}

void
ocpp_status_notification_conf
(
    OCPP *ocpp
)
{
    
}
