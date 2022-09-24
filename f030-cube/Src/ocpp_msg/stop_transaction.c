#include "ocpp_msg/stop_transaction.h"
#include "rapi_msg/get_energy_usage.h"

#include "mjson.h"

void
ocpp_stop_transaction_req
(
	OCPP *ocpp,
	RAPI *rapi
)
{
	rapi_get_energy_usage_req(rapi);
	rapi_send_req(rapi);
	bool resp = rapi_get_resp(rapi, ocpp);
	if (!resp)
		return;

	uint32_t ws;
	rapi_get_energy_usage_resp(rapi, &ws, NULL);
	uint32_t wh = ws / 3600;

	char time[25] = {0};
	// get_rtc_time(rtc, time);

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
		ocpp->_transaction_id
	);

	
	ocpp->pres_msg.type = CALL;
	ocpp->pres_msg.call.action = STOP_TRANSACTION;
	strcpy(ocpp->pres_msg.call.payload, payload);
}

void
ocpp_stop_transaction_conf
(
	OCPP *ocpp
)
{
	if (!strcmp(ocpp->last_msg.ID, ocpp->pres_msg.ID))
		return;

	ocpp->_wait_resp = false;

	if (ocpp->pres_msg.type == CALLERROR)
	{
		return;  // TODO: add handling CALLERRROR
	}

	// AVAILABLE
	// evse_change_state(evse, ocpp, S_AVAILABLE);
	size_t pay_len = strlen(ocpp->pres_msg.call_result.payload);

	char status[10];
	int res_st = mjson_get_string(ocpp->pres_msg.call_result.payload, pay_len, P_ID_INFO_STATUS, status, 10);
	if (res_st < 1)
		return;

	// if (strcmp("Accepted", status))
	// 	usart_ocpp_println_str("TRANSACTION FINISHED\n");
	// else if (strcmp("Rejected", status))
	// 	usart_ocpp_println_str("FAILED TO FINISH TRANSACTION");
	// else
	// 	return;

}

