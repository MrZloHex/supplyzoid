#include "requests/boot_notification.h"


void
ocpp_boot_notification_req
(
	OCPP *ocpp,
	char req[REQ_LEN]
)
{
	char id[37];
	int_to_charset(ocpp->id, id, 1);

	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%Q,%Q:%Q}",
		"chargePointVendor",
		VENDOR,
		"chargePointModel",
		MODEL
	);

	mjson_snprintf
	(
		req, REQ_LEN,
		"[%u,%Q,%Q,%s]",
		CALL,
		id,
		"BootNotification",
		payload
	);

	ocpp->last.type = CALL;
	ocpp->last.ID   = ocpp->id;
	ocpp->last.call.action = BOOT_NOTIFICATION;
	strcpyy(ocpp->last.call.payload, payload);
}

void
ocpp_boot_notification_conf
(
	OCPP *ocpp,
	EVSE *evse
)
{
	if (ocpp->last.ID != ocpp->now.ID)
		return;

	
	printf("HANDLING BOOT NOTIFICATIOSN CONFIRMATION\n");

	ocpp->waiting_for_resp = false;
}