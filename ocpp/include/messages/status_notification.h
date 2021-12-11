#ifndef STATUS_NOTIFICATION_H_
#define STATUS_NOTIFICATION_H_

#include "OCPP.h"

// ChargePointErrorCode
#define NO_ERROR 0
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
    EVSE *evse
);

void
ocpp_status_notification_conf
(
    OCPP *ocpp
);

#endif /* STATUS_NOTIFICATION_H_ */
