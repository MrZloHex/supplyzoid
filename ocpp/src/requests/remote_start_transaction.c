#include "requests/remote_start_transaction.h"

void
ocpp_remote_start_transaction_req
(
	OCPP *ocpp,
	EVSE *evse
)
{
	printf("REMOTE START TRANSACTION\n");
	if (evse->is_transaction)
	{
		// REJECTED
		ocpp_remote_start_transaction_conf(ocpp, RSS_REJECTED);
		ocpp_send_resp(ocpp, CALLRESULT);
	}	
	else
	{
		// ACCEPTED
		ocpp_remote_start_transaction_conf(ocpp, RSS_ACCEPTED);
		ocpp_send_resp(ocpp, CALLRESULT);
	}
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
