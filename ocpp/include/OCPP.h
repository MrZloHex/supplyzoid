#ifndef OCPP_H_

#define OCPP_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#ifndef OCPP
#define OCPP
#endif


#define MESSAGE_TYPE_ID 0
#define MESSAGE_ID      1
#define ACTION          2
#define PAYLOAD         3

#define CALL       2
#define CALLRESULT 3
#define CALLERROR  4



typedef struct
{
    int smth;
} OCPPConfig;


typedef struct
{
    const char messageID[36];

    const char *action;

    const char *payload; // JSON

} OCPPCall;


typedef unsigned char result;

OCPP result
handle_call
(
    OCPPConfig *_cfg,
    const char *_call
);


OCPP OCPPCall
parse_call
(
    const char *_call
);

#endif /* OCPP_H_ */