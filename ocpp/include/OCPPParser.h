#ifndef OCPP_PARSER_H_

#define OCPP_PARSER_H_

#include <stdio.h>
#include <stdlib.h>

#include "stdtypes.h"

#include "OCPPTypes.h"



#define MESSAGE_TYPE_ID 1
#define MESSAGE_ID      2
// CALL
#define C_ACTION        3
#define C_PAYLOAD       4
// CALLRESULT
#define R_PAYLOAD       3
// CALLERROR
#define ERROR_CODE      3
#define ERROR_DSCR      4
#define ERROR_DETAILS   5

typedef unsigned char expected_data;
void
next_data_field(expected_data *data, OCPPMessageType type);



void
make_call
(
    OCPPCall call,
    char    *dest
);

void
make_call_result
(
    OCPPCallResult call_res,
    char          *dest
);

void
make_call_error
(
    OCPPCallError call_err,
    char         *dest
);

OCPPMessageType
determine_message
(
    const char  *_resp,
    const size   length,
    OCPPMessage *message
);


#endif /* OCPP_PARSER_H_ */
