#include "ocpp_msg/remote_start_transaction.h"
#include "ocpp_msg/start_transaction.h"
#include "ocpp_msg/authorize.h"

#include "rapi_msg/get_state.h"
#include "rapi_msg/set_auth_lock.h"

#include "mjson.h"

void
ocpp_remote_start_transaction_req
(
	OCPP *ocpp,
	RAPI *rapi
)
{
	// printf("REMOTE START? TRANSACTION\n");
	// usart_ocpp_println_str("START TRANSACRTION");
	
	size_t pay_len = strlen(ocpp->last_msg.call.payload);

	OCPP_IdTag id_tag;
	int res_id = mjson_get_string(ocpp->last_msg.call.payload, pay_len, P_ID_TAG, id_tag, OCPP_IdTag_Len-1);
	if (res_id == -1)
		return;

	ocpp_authorize_req(ocpp, id_tag);
	ocpp_send_req(ocpp, ACT_AUTHORIZE);
	HAL_UART_Receive_IT(ocpp->uart, (uint8_t *)&ocpp->buffer[0], 1);

	rapi_get_state_req(rapi);
	rapi_send_req(rapi);
	bool resp = rapi_get_resp(rapi, ocpp);
	if (!resp)
		return;

	uint8_t evse_state;
	uint8_t pilot_state;
	rapi_get_state_resp(rapi, &evse_state, NULL, &pilot_state, NULL);
	
	bool reject;
	if (evse_state == EVSE_STATE_B && pilot_state == EVSE_STATE_C)
	{
		ocpp_remote_start_transaction_conf(ocpp, RSS_ACCEPTED);
		reject = false;
	}
	else
	{
		ocpp_remote_start_transaction_conf(ocpp, RSS_REJECTED);
		reject = true;
	}
	ocpp_send_resp(ocpp, CALLRESULT);
	if (reject)
		return;
	
	rapi_set_auth_lock_req(rapi, AUTH_UNLOCKED);
	rapi_send_req(rapi);
	resp = rapi_get_resp(rapi, ocpp);
	if (!resp)
		return;

	ocpp->_is_transaction = true;

	ocpp_start_transaction_req(ocpp, rapi, id_tag);
	ocpp_send_req(ocpp, ACT_START_TRANSACTION);
}

void
ocpp_remote_start_transaction_conf
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

	ocpp->pres_msg.type = CALLRESULT;
	strcpy(ocpp->pres_msg.call_result.payload, payload);
}
