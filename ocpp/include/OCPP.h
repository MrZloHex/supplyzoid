#ifndef OCPP_H_

#define OCPP_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#ifndef OCPP
#define OCPP
#endif



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


// typedef unsigned char result;

OCPP void
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






#define MESSAGE_TYPE_ID 1
#define MESSAGE_ID      2
#define ACTION          3
#define PAYLOAD         4

typedef unsigned char expected_data;
void
next_data_field(expected_data *data);

#endif /* OCPP_H_ */