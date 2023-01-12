#include "ocpp_msg/remote_stop_transaction.h"

#include "mjson.h"

// void
// ocpp_remote_stop_transaction_req
// (
// 	OCPP *ocpp,
// 	RAPI *rapi
// )
// {
// 	// usart_ocpp_print_str("REMOTE STOP TRANSACTION\n");
	
// 	size_t pay_len = strlen(ocpp->last_msg.call.payload);

// 	double transaction_id_d;
// 	int res_id = mjson_get_number(ocpp->last_msg.call.payload, pay_len, P_TRANSACTION_ID, &transaction_id_d);
// 	if (res_id == 0)
// 		return;

// 	int transaction_id = (int)transaction_id_d;

// 	rapi_get_state_req(rapi);
// 	rapi_send_req(rapi);
// 	bool resp = rapi_get_resp(rapi, ocpp);
// 	if (!resp)
// 		return;

// 	uint8_t evse_state;
// 	rapi_get_state_resp(rapi, &evse_state, NULL, NULL, NULL);
	
// 	bool reject;
// 	if (evse_state == EVSE_STATE_C && (ocpp->_transaction_id == transaction_id))
// 	{
// 		ocpp_remote_stop_transaction_conf(ocpp, RSS_ACCEPTED);
// 		reject = false;
// 	}
// 	else
// 	{
// 		ocpp_remote_stop_transaction_conf(ocpp, RSS_REJECTED);
// 		reject = true;
// 	}
// 	ocpp_send_resp(ocpp, CALLRESULT);
// 	if (reject)
// 		return;

// 	rapi_set_auth_lock_req(rapi, AUTH_LOCKED);
// 	rapi_send_req(rapi);
// 	resp = rapi_get_resp(rapi, ocpp);
// 	if (!resp)
// 		return;

// 	ocpp->_is_transaction = false;

// 	ocpp_stop_transaction_req(ocpp, rapi);
// 	ocpp_send_req(ocpp, ACT_STOP_TRANSACTION);
// }

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
