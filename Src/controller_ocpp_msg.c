#include "controller_ocpp_msg.h"

#include "mjson.h"
#include "time.h"

void
ocpp_authorize_req
(
	Controller_OCPP *ocpp,
	OCPP_IdTag *id_tag
)
{
	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%Q}",
		"idTag",
		*id_tag
	);

	ocpp->message.type = CALL;
	ocpp->message.data.call.action = ACT_AUTHORIZE;
	strcpy(ocpp->message.data.call.payload, payload);
}

void
ocpp_boot_notification_req(Controller_OCPP *ocpp)
{
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

	ocpp->message.type = CALL;
	ocpp->message.data.call.action = ACT_BOOT_NOTIFICATION;
	strcpy(ocpp->message.data.call.payload, payload);
}

void
ocpp_start_transaction_req
(
	Controller_OCPP *ocpp,
	uint32_t *wh
)
{
	char time[25] = {0};
	get_rtc_time(ocpp->rtc, time);

	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%u,%Q:%Q,%Q:%lu,%Q:%Q}",
		"connectorId",
		1,
		"idTag",
		ocpp->idtag,
		"meterStart",
		*wh,
		"timestamp",
		time
	);

	ocpp->message.type = CALL;
	ocpp->message.data.call.action = ACT_START_TRANSACTION;
	strcpy(ocpp->message.data.call.payload, payload);
}

void
ocpp_stop_transaction_req
(
	Controller_OCPP *ocpp,
	uint32_t *wh
)
{
	char time[25] = {0};
	get_rtc_time(ocpp->rtc, time);

	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%ld,%Q:%Q,%Q:%d}",
		"meterStop",
		*wh,
		"timestamp",
		time,
		"transactionId",
		ocpp->transaction_id
	);

	
	ocpp->message.type = CALL;
	ocpp->message.data.call.action = ACT_STOP_TRANSACTION;
	strcpy(ocpp->message.data.call.payload, payload);
}

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
		#warning CONNECTOR ID `1` ONLY FOR TESTING STEVE, MUST BE 0
		1,
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
ocpp_meter_values_req
(
	Controller_OCPP *ocpp,
	uint32_t *wh
)
{
	#warning TODO: ADD ANOTHER MEASUREMENTS

	char time[25] = {0};
	get_rtc_time(ocpp->rtc, time);

	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%d,%Q:[{%Q:%Q,%Q:[{%Q:%ld,%Q:%Q}]}]}",
		"connectorId",
		0,
		"meterValue",
		"timestamp",
		time,
		"sampledValue",
		"value", *wh,
		"unit", "Wh"
	);

	ocpp->message.type = CALL;
	ocpp->message.data.call.action = ACT_METER_VALUES;
	strcpy(ocpp->message.data.call.payload, payload);
}


void
ocpp_remote_start_transaction_conf
(
	Controller_OCPP *ocpp,
	bool *status
)
{
	char payload[PAYLOAD_LEN];
	if (*status)
	{
		mjson_snprintf
		(
			payload, PAYLOAD_LEN,
			"{%Q:%Q}",
			"status",
			"Accepted"
		);
	}
	else 
	{
		mjson_snprintf
		(
			payload, PAYLOAD_LEN,
			"{%Q:%Q}",
			"status",
			"Rejected"
		);
	}

	ocpp->message.type = CALLRESULT;
	strcpy(ocpp->message.data.call_result.payload, payload);
}

void
ocpp_remote_stop_transaction_conf
(
	Controller_OCPP *ocpp,
	bool *status
)
{
	char payload[PAYLOAD_LEN];
	if (*status)
	{
		mjson_snprintf
		(
			payload, PAYLOAD_LEN,
			"{%Q:%Q}",
			"status",
			"Accepted"
		);
	}
	else
	{
		mjson_snprintf
		(
			payload, PAYLOAD_LEN,
			"{%Q:%Q}",
			"status",
			"Rejected"
		);
	}

	ocpp->message.type = CALLRESULT;
	strcpy(ocpp->message.data.call_result.payload, payload);
}

const static char k_AS_ACCEPTED[]  = "Accepted";
const static char k_AS_REJECTED[]  = "Rejected";
const static char k_AS_SCHEDULED[] = "Scheduled";
const static char *k_AS_STRINGS[3] = { k_AS_ACCEPTED, k_AS_REJECTED, k_AS_SCHEDULED };

void
ocpp_change_availabilty_conf
(
	Controller_OCPP *ocpp,
    OCPP_AvailabilityStatus *status
)
{
	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%Q}",
		"status",
		k_AS_STRINGS[*status]
	);

	ocpp->message.type = CALLRESULT;
	strcpy(ocpp->message.data.call_result.payload, payload);
}

void
ocpp_reset_conf
(
	Controller_OCPP *ocpp,
	bool *status
)
{
	char payload[PAYLOAD_LEN];
	if (*status)
	{
		mjson_snprintf
		(
			payload, PAYLOAD_LEN,
			"{%Q:%Q}",
			"status",
			"Accepted"
		);
	}
	else
	{
		mjson_snprintf
		(
			payload, PAYLOAD_LEN,
			"{%Q:%Q}",
			"status",
			"Rejected"
		);
	}

	ocpp->message.type = CALLRESULT;
	strcpy(ocpp->message.data.call_result.payload, payload);
}

const static char k_US_ACCEPTED[]			= "Accepted";
const static char k_US_FAILED[]  			= "Rejected";
const static char k_US_NOT_SUPPORTED[]		= "NotSupported";
const static char k_US_VERSION_MISMATCH[]	= "VersionMismatch";
const static char *k_US_STRINGS[4] = { k_US_ACCEPTED, k_US_FAILED, k_US_NOT_SUPPORTED, k_US_VERSION_MISMATCH };

void
ocpp_send_local_list_conf
(
	Controller_OCPP *ocpp,
	OCPP_UpdateStatus *status
)
{
	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%Q}",
		"status",
		k_US_STRINGS[*status]
	);

	ocpp->message.type = CALLRESULT;
	strcpy(ocpp->message.data.call_result.payload, payload);
}

void
ocpp_get_local_list_version_conf(Controller_OCPP *ocpp)
{
	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%u}",
		"listVersion",
		ocpp->list.version
	);

	ocpp->message.type = CALLRESULT;
	strcpy(ocpp->message.data.call_result.payload, payload);
}