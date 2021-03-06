#include "ocpp_msg/start_transaction.h"
#include "rapi_msg/get_energy_usage.h"

#include "RTC.h"
#include "usart_ocpp.h"

void
ocpp_start_transaction_req
(
	OCPP *ocpp,
	RAPI *rapi,
	STM32RTC *rtc,
	char id_tag[21]
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
		"{%Q:%u,%Q:%Q,%Q:%lu,%Q:%Q}",
		"connectorId",
		1,
		"idTag",
		id_tag,
		"meterStart",
		wh,
		"timestamp",
		time
	);

	ocpp->now.type = CALL;
	ocpp->now.call.action = START_TRANSACTION;
	strcpyy(ocpp->now.call.payload, payload);
}

void
ocpp_start_transaction_conf
(
	OCPP *ocpp
)
{
	if (!strcmpp(ocpp->now.ID, ocpp->last.ID))
		return;

	ocpp->waiting_for_resp = false;

	if (ocpp->now.type == CALLERROR)
	{
		return;  // TODO: add handling CALLERRROR
	}
	
	size pay_len = strlenn(ocpp->now.call_result.payload);

	double transaction_id;
	int res_int = mjson_get_number(ocpp->now.call_result.payload, pay_len, P_TRANSACTION_ID, &transaction_id);
	if (res_int == 0)
		return;

	char status[10];
	int res_st = mjson_get_string(ocpp->now.call_result.payload, pay_len, P_ID_INFO_STATUS, status, 10);
	if (res_st < 1)
		return;

	if (strcmpp("Accepted", status))
	{
		ocpp->transactionID = (int)transaction_id;
		// usart_ocpp_println_str("TRANSACTION CONFIRMED");
	}
	else if (strcmpp("Rejected", status))
	{
		// usart_ocpp_println_str("TRANSACTION REJECTED");
	}
	else
	{
		return;
	}

		
}
