#include "requests/remote_start_transaction.h"

void
ocpp_remote_start_transaction_req
(
	OCPP *ocpp,
	EVSE *evse
)
{
	printf("REMOTE START TRANSACTION\n");
	
	size pay_len = strlenn(ocpp->last.call.payload);

	char id_tag[21];
	int res_id = mjson_get_string(ocpp->last.call.payload, pay_len, P_ID_TAG, id_tag, 20);
	if (res_id == -1)
		return;

	if (evse->is_transaction)
		ocpp_remote_start_transaction_conf(ocpp, RSS_REJECTED);
	else
		ocpp_remote_start_transaction_conf(ocpp, RSS_ACCEPTED);
	
	ocpp_send_resp(ocpp, CALLRESULT);
	
	// TRANSACTION
	evse_start_transaction(evse);

	ocpp_start_transaction_req(ocpp, evse, id_tag);
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
