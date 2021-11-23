#ifndef OCPP_H_

#define OCPP_H_

#include <stdio.h>

#include "stringg.h"
#include "convert.h"
#include "JSON.h"
#include "EVSE.h"

#include "OCPPTypes.h"


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
	OCPP *ocpp,
	const char *str,
	const size length
);

#endif /* OCPP_H_ */
