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

	ocpp->waiting_for_resp = false;

	if (ocpp->now.type == CALLERROR)
	{
		return;  // TODO: add handling CALLERRROR
	}
	
	size pay_len = strlenn(ocpp->now.call_result.payload);

	double interval;
	mjson_get_number(ocpp->now.call_result.payload, pay_len, P_INTERVAL, &interval);
	evse->heartbeat_time = (time_t)interval;
	printf("INTERVAL: %lu\n", evse->heartbeat_time);

	char status[9];
	mjson_get_string(ocpp->now.call_result.payload, pay_len, P_STATUS, status, 9);

	if (strcmpp(status, "Accepted"))
		evse->booted = true;
	else if (strcmpp(status, "Pending") || strcmpp(status, "Rejected"))
		evse->booted = false;
	else
		return;
}