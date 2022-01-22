#include "ocpp_msg/data_transfer.h"


void
ocpp_data_transfer_req
(
	OCPP *ocpp
)
{
	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%Q}",
		"vendorId",
		"PROTOTYPE"
		// ADD DATA
	);

	ocpp->now.type = CALL;
	ocpp->now.ID   = ocpp->id;
	ocpp->now.call.action = DATA_TRANSFER;
	strcpyy(ocpp->now.call.payload, payload);
}

void
ocpp_data_transfer_conf
(
	OCPP *ocpp
)
{
	if (ocpp->last.ID != ocpp->now.ID)
		return;

	ocpp->waiting_for_resp = false;
}
