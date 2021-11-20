#ifndef OCPP_H_

#define OCPP_H_

#include <stdio.h>

#include "stringg.h"
#include "convert.h"
#include "JSON.h"
#include "EVSE.h"


#ifndef OCPP
#define OCPP
#endif



#define CALL       2
#define CALLRESULT 3
#define CALLERROR  4
typedef unsigned char OCPPType;

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
next_data_field(expected_data *data, OCPPType type);




typedef struct
{
    const unsigned int   messageID;
    const char *action;
    const char *payload; // JSON

} OCPPCall;

#define OCPPCallResult_Fmt "%d  %s"
#define OCPPCallResult_Arg(cr) (cr).messageID, (cr).payload

typedef struct
{
    const unsigned int   messageID;
    const char *payload;
} OCPPCallResult;

typedef struct
{
    const unsigned int   messageID;
    const char  error_code[30];
    const char  error_dscr[255];
    const char *error_details;
} OCPPCallError;

typedef union
{
    OCPPCallResult call_result;
    OCPPCallError  call_error;
} OCPPResponse;


// typedef unsigned char result;

// OCPP void
// handle_call
// (
//     OCPPConfig *_cfg,
//     const char *_call
// );


// OCPP OCPPCall
// parse_call
// (
//     const char *_call
// );






OCPP void
ocpp_init();

OCPP void
ocpp_update
(
    EVSE *evse
);



OCPP void
make_call
(
    OCPPCall call_cfg,
    char       *_dest
);

OCPP int
handle_response
(
    const char *_resp,
    OCPPResponse *response
);



#endif /* OCPP_H_ */