#include "requests/start_transaction.h"


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
	OCPP *ocpp
)
{
	printf("HANDLE CONFIRMATION FOR START TRANSACTION\n");
}
