#include "messages/start_transaction.h"


void
ocpp_start_transaction_req
(
	OCPP *ocpp,
	EVSE *evse,
	char id_tag[21]
)
{
	char id[37];
	int_to_charset(ocpp->id, id, 1);

	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%u,%Q:%Q,%Q:%ld,%Q:%Q}",
		"connectorId",
		1,
		"idTag",
		id_tag,
		"meterStart",
		evse->meter_value,
		"timestamp",
		// TIMESTAMP
		"18.06.2021.687"
	);

	ocpp->now.type = CALL;
	ocpp->now.ID   = ocpp->id;
	ocpp->now.call.action = START_TRANSACTION;
	strcpyy(ocpp->now.call.payload, payload);
}

void
ocpp_start_transaction_conf
(
	OCPP *ocpp,
	EVSE *evse
)
{
	if (ocpp->last.ID != ocpp->now.ID)
		return;

	ocpp->waiting_for_resp = false;

	if (ocpp->now.type == CALLERROR)
	{
		return;  // TODO: add handling CALLERRROR
	}
	
	size pay_len = strlenn(ocpp->now.call_result.payload);

	double transaction_id;
	int res_int = mjson_get_number(ocpp->now.call_result.payload, pay_len, P_TRANSACTION_ID, &transaction_id);
	if (res_int == 0)
		return;

	evse->transactionID = (int)transaction_id;
		
	printf("TRANSACTION CONFIRMED\n");
}
