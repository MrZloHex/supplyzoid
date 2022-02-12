#include "ocpp_msg/boot_notification.h"


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

	ocpp->now.type = CALL;
	ocpp->now.ID   = ocpp->id;
	ocpp->now.call.action = BOOT_NOTIFICATION;
	strcpyy(ocpp->now.call.payload, payload);
}

void
ocpp_boot_notification_conf
(
	OCPP *ocpp,
	RAPI *rapi,
	STM32RTC *rtc
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
	int res_int = mjson_get_number(ocpp->now.call_result.payload, pay_len, P_INTERVAL, &interval);
	if (res_int == 0)
		return;
	

	char status[9];
	int res_st = mjson_get_string(ocpp->now.call_result.payload, pay_len, P_STATUS, status, 9);
	if (res_st == -1)
		return;

	if (strcmpp(status, "Accepted"))
		ocpp->booted = true;
	else if (strcmpp(status, "Pending") || strcmpp(status, "Rejected"))
		ocpp->booted = false;
	else
		return;

	char time[25];
	int res_time = mjson_get_string(ocpp->now.call_result.payload, pay_len, P_CURRENT_TIME, time, 25);
	if (res_time == -1)
		return;

	adjust_rtc_time(rtc, time);


	serial_println_str("BOOTED");
}