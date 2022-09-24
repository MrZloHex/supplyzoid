#include "ocpp_msg/status_notification.h"

#include "mjson.h"

char *ChargePointErrorCode[] = {
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

void
ocpp_status_notification_req
(
    OCPP *ocpp,
    RAPI *rapi
)
{
	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%d,%Q:%Q,%Q:%Q}",
		"connectorId",
		1,
		"errorCode",
		ChargePointErrorCode[NO_ERROR],
		"status"
		// evse_get_state(evse)
	);

	ocpp->pres_msg.type = CALL;
	ocpp->pres_msg.call.action = STATUS_NOTIFICATION;
	strcpy(ocpp->pres_msg.call.payload, payload);
}

void
ocpp_status_notification_conf
(
    OCPP *ocpp
)
{
    
}
