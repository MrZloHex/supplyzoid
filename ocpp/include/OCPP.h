#ifndef OCPP_H_

#define OCPP_H_

#include <stdio.h>

#include "stringg.h"
#include "convert.h"
#include "JSON.h"
#include "EVSE.h"

#include "OCPPTypes.h"
#include "OCPPStates.h"


typedef struct
{
	OCPPMessage now;
	OCPPMessage last;
} OCPP;


void
ocpp_init(OCPP *ocpp);

void
ocpp_free(OCPP *ocpp);

void
ocpp_update
(
	OCPP *ocpp,
	EVSE *evse
);

void
ocpp_handle_message
(
 	OCPP *ocpp,
	EVSE *evse,
	const char *str,
	const size length
);



OCPPMessageType
ocpp_determine_message_type
(
	const char *str,
	const size length
);

OCPPMessageID
ocpp_get_message_id
(
	const char *str,
	const size length
);


OCPPCallAction
ocpp_get_action
(
	const char *str,
	const size length
);

OCPPResult
ocpp_get_payload
(
	OCPPMessageType type,
	const char *str,
	const size length,
	char *dst
);

#endif /* OCPP_H_ */
