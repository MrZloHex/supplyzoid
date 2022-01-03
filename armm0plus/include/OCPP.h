#ifndef OCPP_H_
#define OCPP_H_

#define VENDOR "EV Solutions"
#define MODEL  "PROTOTYPE"

#include "stringg.h"
#include "convert.h"
#include "JSON.h"
#include "EVSE.h"

#include "OCPPTypes.h"
#include "OCPPStates.h"




void
ocpp_init(OCPP *ocpp);

void
ocpp_free(OCPP *ocpp);

void
ocpp_next(OCPP *ocpp);

void
ocpp_update
(
	OCPP *ocpp,
	EVSE *evse,
	STM32RTC *rtc
);

void
ocpp_handle_message
(
 	OCPP *ocpp,
	EVSE *evse,
	STM32RTC *rtc,
	const char *str,
	const size length
);

OCPPResult
ocpp_parse_message
(
	OCPP *ocpp,
	const char *str,
	const size length
);


void
ocpp_send_req
(
	OCPP *ocpp,
	OCPPCallAction action
);

void
ocpp_send_resp
(
	OCPP *ocpp,
	OCPPMessageType type
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

OCPPCallErrorCode
ocpp_get_call_error_code
(
	const char *str,
	const size length
);

OCPPResult
ocpp_get_call_error_descr
(
	const char *str,
	const size length,
	char *dscr
);

#endif /* OCPP_H_ */
