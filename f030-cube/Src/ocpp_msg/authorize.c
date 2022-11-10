#include "ocpp_msg/authorize.h"

#include "mjson.h"

void
ocpp_authorize_req
(
	OCPP *ocpp,
	OCPP_IdTag id_tag
)
{
    
	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%Q}",
		"idTag",
		id_tag
	);

	ocpp->pres_msg.type = CALL;
	ocpp->pres_msg.call.action = ACT_AUTHORIZE;
	strcpy(ocpp->pres_msg.call.payload, payload);
}

void
ocpp_authorize_conf
(
	OCPP *ocpp
)
{

}
