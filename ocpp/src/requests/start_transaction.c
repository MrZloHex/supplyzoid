#include "requests/start_transaction.h"

void
ocpp_start_transaction_req
(
	OCPP *ocpp,
	EVSE *evse,
	char req[REQ_LEN]
)
{
	char id[37];
	int_to_charset(ocpp->id, id, 1);

	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%u,%Q:%Q,%Q:%d,%Q:%Q}",
		"connectorId",
		1,
		"idTag",
		// IDTAG,
		"TAG",
		"meterStart",
		// METER_STATE,
		0,
		"timestamp",
		// TIMESTAMP
		"18.06.2021.687"
	);

	mjson_snprintf
	(
		req, REQ_LEN,
		"[%u,%Q,%Q,%s]",
		CALL,
		id,
		"StartTransaction",
		payload
	);

	ocpp->last.type = CALL;
	ocpp->last.ID   = ocpp->id;
	ocpp->last.call.action = START_TRANSACTION;
	strcpyy(ocpp->last.call.payload, payload);
}
