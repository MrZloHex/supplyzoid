#ifndef OCPP_H_

#define OCPP_H_

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


typedef unsigned char result;

OCPP result
handle_call
(
    OCPPConfig *_cfg,
    const char *_call
);

#endif /* OCPP_H_ */