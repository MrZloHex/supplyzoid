#include "ocpp_msg/status_notification.h"

#include "mjson.h"

const static char *ChargePointErrorCode[] = {
	"NoError",
	"ConnectorLockFailure",
	"EVCommunicationError",
	"GroundFailure",
	"HighTemperature",
	"InternalError",
	"LocalListConflict",
	"OtherError",
	"OverCurrentFailure",
	"OverVoltage",
	"PowerMeterFailure",
	"PowerSwitchFailure",
	"ReaderFailure",
	"ResetFailure",
	"UnderVoltage",
	"WeakSignal"
};

const static char *ChargePointStatus[] = {
	"",
	"Available",
	"Preparing",
	"Charging",
	"SuspendedEVSE",
	"SuspendedEV",
	"Finishing",
	"Reserved",
	"Unavailable",
	"Faulted"
};

void
ocpp_status_notification_req
(
    Controller_OCPP *ocpp,
	OCPP_ChargePointStatus *status,
	OCPP_ChargePointErrorCode *error
)
{
	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%d,%Q:%Q,%Q:%Q}",
		"connectorId",
		0,
		"errorCode",
		ChargePointErrorCode[*error],
		"status",
		ChargePointStatus[*status]
	);

	ocpp->message.type = CALL;
	ocpp->message.data.call.action = ACT_STATUS_NOTIFICATION;
	strcpy(ocpp->message.data.call.payload, payload);
}

void
ocpp_status_notification_conf
(
    Controller_OCPP *ocpp
)
{
    
}
