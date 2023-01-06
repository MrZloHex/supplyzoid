#include "ocpp_msg/authorize.h"

#include "mjson.h"

void
ocpp_authorize_req
(
	Controller_OCPP *ocpp,
	OCPP_IdTag *id_tag
)
{
	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%Q}",
		"idTag",
		*id_tag
	);

	ocpp->message.type = CALL;
	ocpp->message.data.call.action = ACT_AUTHORIZE;
	strcpy(ocpp->message.data.call.payload, payload);
}

void
ocpp_authorize_conf
(
	Controller_OCPP *ocpp
)
{

}
