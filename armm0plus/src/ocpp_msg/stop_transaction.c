#include "ocpp_msg/stop_transaction.h"
#include "rapi_msg/get_energy_usage.h"

#include "RTC.h"
#include "usart_ocpp.h"

void
ocpp_stop_transaction_req
(
	OCPP *ocpp,
	RAPI *rapi,
	STM32RTC *rtc
)
{
	rapi_get_energy_usage_req(rapi);
	rapi_send_req(rapi);
	bool resp = rapi_get_resp(rapi, ocpp);
	if (!resp)
		return;

	u32 ws;
	rapi_get_energy_usage_resp(rapi, &ws, NULL);
	u32 wh = ws / 3600;

	char time[25] = {0};
	get_rtc_time(rtc, time);

	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%ld,%Q:%Q,%Q:%d}",
		"meterStop",
		wh,
		"timestamp",
		time,
		"transactionId",
		ocpp->transactionID
	);

	
	ocpp->now.type = CALL;
	ocpp->now.call.action = STOP_TRANSACTION;
	strcpyy(ocpp->now.call.payload, payload);
}

void
ocpp_stop_transaction_conf
(
	OCPP *ocpp
)
{
	if (!strcmpp(ocpp->last.ID, ocpp->now.ID))
		return;

	ocpp->waiting_for_resp = false;

	if (ocpp->now.type == CALLERROR)
	{
		return;  // TODO: add handling CALLERRROR
	}

	// AVAILABLE
	// evse_change_state(evse, ocpp, S_AVAILABLE);
	size pay_len = strlenn(ocpp->now.call_result.payload);

	char status[10];
	int res_st = mjson_get_string(ocpp->now.call_result.payload, pay_len, P_ID_INFO_STATUS, status, 10);
	if (res_st < 1)
		return;

	// if (strcmpp("Accepted", status))
	// 	usart_ocpp_println_str("TRANSACTION FINISHED\n");
	// else if (strcmpp("Rejected", status))
	// 	usart_ocpp_println_str("FAILED TO FINISH TRANSACTION");
	// else
	// 	return;

}

