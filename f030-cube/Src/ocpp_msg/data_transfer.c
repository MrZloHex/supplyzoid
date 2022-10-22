#include "ocpp_msg/data_transfer.h"

#include "mjson.h"

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

	ocpp->pres_msg.type = CALL;
	ocpp->pres_msg.call.action = ACT_DATA_TRANSFER;
	strcpy(ocpp->pres_msg.call.payload, payload);
}

void
ocpp_data_transfer_conf
(
	OCPP *ocpp
)
{
	if (strcmp(ocpp->last_msg.ID, ocpp->pres_msg.ID) != 0)
		return;

	ocpp->_wait_resp = false;
}
