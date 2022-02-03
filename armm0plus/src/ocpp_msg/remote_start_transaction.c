#include "ocpp_msg/remote_start_transaction.h"
#include "ocpp_msg/start_transaction.h"

#include "rapi_msg/get_state.h"


void
ocpp_remote_start_transaction_req
(
	OCPP *ocpp,
	RAPI *rapi
)
{
	// printf("REMOTE START? TRANSACTION\n");
	serial_println_str("START TRANSACRTION");
	
	size pay_len = strlenn(ocpp->last.call.payload);

	char id_tag[21];
	int res_id = mjson_get_string(ocpp->last.call.payload, pay_len, P_ID_TAG, id_tag, 20);
	if (res_id == -1)
		return;

	rapi_get_state_req(rapi);
	rapi_send_req(rapi);
	bool resp = rapi_get_resp(rapi, ocpp);
	if (!resp)
		return;

	u8 evse_state;
	rapi_get_state_resp(rapi, &evse_state, NULL, NULL, NULL);
	
	bool reject;
	if (evse_state != EVSE_STATE_B)
	{
		ocpp_remote_start_transaction_conf(ocpp, RSS_REJECTED);
		reject = true;
	}
	else
	{
		ocpp_remote_start_transaction_conf(ocpp, RSS_ACCEPTED);
		reject = false;
	}
	ocpp_send_resp(ocpp, CALLRESULT);
	if (reject)
		return;
	
	// // PREPARING
	// // evse_change_state(evse, ocpp, S_PREPARING);
	// // TRANSACTION
	// // evse_start_transaction(evse);
	// // CHARGING
	// // evse_change_state(evse, ocpp, S_CHARGING);

	ocpp_start_transaction_req(ocpp, rapi, id_tag);
	ocpp_send_req(ocpp, START_TRANSACTION);
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

	ocpp->now.ID = ocpp->last.ID;
	ocpp->now.type = CALLRESULT;
	strcpyy(ocpp->now.call_result.payload, payload);
}
