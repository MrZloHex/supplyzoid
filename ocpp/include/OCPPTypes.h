#ifndef OCPP_TYPES_H_

#define OCPP_TYPES_H_

#define ERROR      0
#define CALL       2
#define CALLRESULT 3
#define CALLERROR  4
typedef unsigned char OCPPType;


typedef struct
{
    const unsigned int   messageID;
    const char          *action;
    const char          *payload; // JSON

} OCPPCall;

typedef struct
{
    const unsigned int   messageID;
    const char          *payload;
} OCPPCallResult;

typedef struct
{
    const unsigned int   messageID;
    const char           error_code[30];
    const char           error_dscr[255];
    const char          *error_details;
} OCPPCallError;

typedef union
{
    OCPPCall       call;
    OCPPCallResult call_result;
    OCPPCallError  call_error;
} OCPPMessage;



#endif /* OCPP_TYPES_H_ */