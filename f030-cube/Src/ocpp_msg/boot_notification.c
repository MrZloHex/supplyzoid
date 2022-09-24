#include "ocpp_msg/boot_notification.h"

#include "mjson.h"

void
ocpp_boot_notification_req
(
	OCPP *ocpp
)
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

	ocpp->pres_msg.type = CALL;
	ocpp->pres_msg.call.action = BOOT_NOTIFICATION;
	strcpy(ocpp->pres_msg.call.payload, payload);
}

void
ocpp_boot_notification_conf
(
	OCPP *ocpp,
	RAPI *rapi
)
{
	if (!strcmp(ocpp->pres_msg.ID, ocpp->last_msg.ID))
		return;

	ocpp->_wait_resp = false;

	if (ocpp->pres_msg.type == CALLERROR)
	{
		return;  // TODO: add handling CALLERRROR
	}
	
	size_t pay_len = strlen(ocpp->pres_msg.call_result.payload);

	double interval;
	int res_int = mjson_get_number(ocpp->pres_msg.call_result.payload, pay_len, P_INTERVAL, &interval);
	if (res_int == 0)
		return;
	

	char status[9];
	int res_st = mjson_get_string(ocpp->pres_msg.call_result.payload, pay_len, P_STATUS, status, 9);
	if (res_st == -1)
		return;

	if (strcmp(status, "Accepted"))
		ocpp->_booted = true;
	else if (strcmp(status, "Pending") || strcmp(status, "Rejected"))
		ocpp->_booted = false;
	else
		return;

	char time[25];
	int res_time = mjson_get_string(ocpp->pres_msg.call_result.payload, pay_len, P_CURRENT_TIME, time, 25);
	if (res_time == -1)
		return;

	// adjust_rtc_time(rtc, time);

}