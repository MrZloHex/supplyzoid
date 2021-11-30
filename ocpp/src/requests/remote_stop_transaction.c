#include "requests/remote_stop_transaction.h"
#include "requests/stop_transaction.h"


void
ocpp_remote_stop_transaction_req
(
	OCPP *ocpp,
	EVSE *evse
)
{
	printf("REMOTE STOP TRANSACTION\n");
	
	size pay_len = strlenn(ocpp->last.call.payload);

	double transaction_id_d;
	int res_id = mjson_get_number(ocpp->last.call.payload, pay_len, P_TRANSACTION_ID, &transaction_id_d);
	if (res_id == 0)
		return;
	int transaction_id = (int)transaction_id_d;

	if (evse->is_transaction && (evse->transactionID == transaction_id))
		ocpp_remote_stop_transaction_conf(ocpp, RSS_ACCEPTED);
	else
		ocpp_remote_stop_transaction_conf(ocpp, RSS_REJECTED);
	ocpp_send_resp(ocpp, CALLRESULT);

	// TRANSACTION
	evse_stop_transaction(evse);

	

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

	ocpp->now.ID = ocpp->last.ID;
	ocpp->now.type = CALLRESULT;
	strcpyy(ocpp->now.call_result.payload, payload);
}
