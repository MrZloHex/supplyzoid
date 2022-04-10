#include "ocpp_msg/remote_stop_transaction.h"
#include "ocpp_msg/stop_transaction.h"

#include "rapi_msg/get_state.h"
#include "rapi_msg/set_auth_lock.h"
#include "usart_ocpp.h"

void
ocpp_remote_stop_transaction_req
(
	OCPP *ocpp,
	RAPI *rapi,
	STM32RTC *rtc
)
{
	// usart_ocpp_print_str("REMOTE STOP TRANSACTION\n");
	
	size pay_len = strlenn(ocpp->last.call.payload);

	double transaction_id_d;
	int res_id = mjson_get_number(ocpp->last.call.payload, pay_len, P_TRANSACTION_ID, &transaction_id_d);
	if (res_id == 0)
		return;

	int transaction_id = (int)transaction_id_d;

	rapi_get_state_req(rapi);
	rapi_send_req(rapi);
	bool resp = rapi_get_resp(rapi, ocpp);
	if (!resp)
		return;

	u8 evse_state;
	rapi_get_state_resp(rapi, &evse_state, NULL, NULL, NULL);
	
	bool reject;
	if (evse_state == EVSE_STATE_C && (ocpp->transactionID == transaction_id))
	{
		ocpp_remote_stop_transaction_conf(ocpp, RSS_ACCEPTED);
		reject = false;
	}
	else
	{
		ocpp_remote_stop_transaction_conf(ocpp, RSS_REJECTED);
		reject = true;
	}
	ocpp_send_resp(ocpp, CALLRESULT);
	if (reject)
		return;

	rapi_set_auth_lock_req(rapi, AUTH_LOCKED);
	rapi_send_req(rapi);
	resp = rapi_get_resp(rapi, ocpp);
	if (!resp)
		return;

	ocpp_stop_transaction_req(ocpp, rapi, rtc);
	ocpp_send_req(ocpp, STOP_TRANSACTION);
}

void
ocpp_remote_stop_transaction_conf
(
	OCPP *ocpp,
	unsigned char status
)
{

	char payload[PAYLOAD_LEN];
	if (status == RSS_ACCEPTED)
	{
		mjson_snprintf
		(
			payload, PAYLOAD_LEN,
			"{%Q:%Q}",
			"status",
			"Accepted"
		);
	}
	else if (status == RSS_REJECTED)
	{
		mjson_snprintf
		(
			payload, PAYLOAD_LEN,
			"{%Q:%Q}",
			"status",
			"Rejected"
		);
	}

	ocpp->now.type = CALLRESULT;
	strcpyy(ocpp->now.call_result.payload, payload);
}
