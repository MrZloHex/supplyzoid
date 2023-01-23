#include "ocpp_msg/change_availability.h"

#include "mjson.h"

void
ocpp_change_availability_req
(
	Controller_OCPP *ocpp
)
{
    
}

const static char k_AS_ACCEPTED[]  = "Accepted";
const static char k_AS_REJECTED[]  = "Rejected";
const static char k_AS_SCHEDULED[] = "Scheduled";
const static char *k_AS_STRINGS[3] = { k_AS_ACCEPTED, k_AS_REJECTED, k_AS_SCHEDULED };

void
ocpp_change_availabilty_conf
(
	Controller_OCPP *ocpp,
    OCPP_AvailabilityStatus *status
)
{
	char payload[PAYLOAD_LEN];
	mjson_snprintf
	(
		payload, PAYLOAD_LEN,
		"{%Q:%Q}",
		"status",
		k_AS_STRINGS[*status]
	);

	ocpp->message.type = CALL;
	ocpp->message.data.call.action = ACT_CHANGE_AVAILABILITY;
	strcpy(ocpp->message.data.call.payload, payload);
}