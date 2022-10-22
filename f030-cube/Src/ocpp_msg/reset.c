#include "ocpp_msg/reset.h"
#include "ocpp_msg/stop_transaction.h"
#include "rapi_msg/reset.h"
#include "rapi_msg/get_state.h"
#include "rapi_msg/set_auth_lock.h"

#include "mjson.h"

void
ocpp_reset_req
(
	OCPP *ocpp,
	RAPI *rapi
)
{
	size_t pay_len = strlen(ocpp->last_msg.call.payload);

	char reset_type_str[21];
	int res_id = mjson_get_string
	(
		ocpp->last_msg.call.payload,
		pay_len,
		P_RESET_TYPE,
		reset_type_str,
		20
	);
	if (res_id == -1)
		return;

	uint8_t reset_type = 0;
	if (strcmp(reset_type_str, "Hard") == 0)
		reset_type = 1;
	else if (strcmp(reset_type_str, "Soft") == 0)
		reset_type = 2;
	else
		return;

	ocpp_reset_conf(ocpp, RSS_ACCEPTED);
	ocpp_send_resp(ocpp, CALLRESULT);

	if (reset_type == 2)
	{
		rapi_get_state_req(rapi);
		rapi_send_req(rapi);
		bool resp = rapi_get_resp(rapi, ocpp);
		if (!resp)
			return;

		uint8_t evse_state;
		rapi_get_state_resp(rapi, &evse_state, NULL, NULL, NULL);
		if (evse_state == EVSE_STATE_C)
		{
			// STOP CHARGING
			rapi_set_auth_lock_req(rapi, AUTH_LOCKED);
			rapi_send_req(rapi);
			rapi_get_resp(rapi, ocpp);
			ocpp_stop_transaction_req(ocpp, rapi);
		}
	}

	rapi_reset_req(rapi);
	rapi_send_req(rapi);
}

void
ocpp_reset_conf
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
			"type",
			"Accepted"
		);
	}
	else if (status == RSS_REJECTED)
	{
		mjson_snprintf
		(
			payload, PAYLOAD_LEN,
			"{%Q:%Q}",
			"type",
			"Rejected"
		);
	}

	ocpp->pres_msg.type = CALLRESULT;
	strcpy(ocpp->pres_msg.call_result.payload, payload);
}
