#include "messages/stop_transaction.h"


void
ocpp_stop_transaction_req
(
	OCPP *ocpp,
	EVSE *evse
)
{
	char id[37];
	int_to_charset(ocpp->id, id, 1);

	char payload[PAYLOAD_LEN];
	printf("%ld\n", evse->meter_value);
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%ld,%Q:%Q,%Q:%d}",
		"meterStop",
		evse->meter_value,
		"timestamp",
		// TIMESTAMP
		"18.06.2021.687",
		"transactionId",
		evse->transactionID
	);

	
	ocpp->now.type = CALL;
	ocpp->now.ID   = ocpp->id;
	ocpp->now.call.action = STOP_TRANSACTION;
	strcpyy(ocpp->now.call.payload, payload);
}

void
ocpp_stop_transaction_conf
(
	OCPP *ocpp
)
{
	if (ocpp->last.ID != ocpp->now.ID)
		return;

	ocpp->waiting_for_resp = false;

	if (ocpp->now.type == CALLERROR)
	{
		return;  // TODO: add handling CALLERRROR
	}

	printf("TRANSACTION FINISHED\n");
}

