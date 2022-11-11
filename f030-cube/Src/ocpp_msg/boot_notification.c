#include "ocpp_msg/boot_notification.h"

#include "mjson.h"
#include "time.h"
#include "serial.h"

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
ocpp_boot_notification_conf(Controller_OCPP *ocpp)
{
	// uprintf(ocpp->uart, 100, 10,"BOO\n");
	if (strcmp(ocpp->message.id, ocpp->message.id) != 0)
	{
	// uprintf(ocpp->uart, 100, 40, "I: `%s`, `%s`\n", ocpp->pres_msg.ID, ocpp->last_msg.ID);
		return;
	}

	// ocpp->_wait_resp = false;

	if (ocpp->message.type == CALLERROR)
	{
		return;  // TODO: add handling CALLERRROR
	}
	
	size_t pay_len = strlen(ocpp->message.data.call_result.payload);

	double interval;
	int res_int = mjson_get_number(ocpp->message.data.call_result.payload, pay_len, P_INTERVAL, &interval);
	if (res_int == 0)
		return;
	

	char status[9];
	int res_st = mjson_get_string(ocpp->message.data.call_result.payload, pay_len, P_STATUS, status, 9);
	if (res_st == -1)
		return;

	if (strcmp(status, "Accepted") == 0)
	{}
		// ocpp->_booted = true;
	else if (strcmp(status, "Pending") == 0 || strcmp(status, "Rejected") == 0)
	{}
		// ocpp->_booted = false;
	else
		return;

	char time[25];
	int res_time = mjson_get_string(ocpp->message.data.call_result.payload, pay_len, P_CURRENT_TIME, time, 25);
	if (res_time == -1)
		return;

	uprintf(ocpp->uart, 1000, 10,"TED\n");
	adjust_rtc_time(ocpp->rtc, time);
}