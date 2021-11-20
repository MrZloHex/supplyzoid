#ifndef OCPP_TYPES_H_

#define OCPP_TYPES_H_

#define ERROR      0
// [2,"19223201","BootNotification",{"reason":"PowerUp","chargingStation":{"model":"SingleSocketCharger","vendorName":"VendorX"}}]
#define CALL       2
// [3,"19223201",{"currentTime":"2013-02-01T20:53:32.486Z","interval":300,"status":"Accepted"}]
#define CALLRESULT 3
// [4,"19223201","GenericError","Any other error not covered by the previous ones"{}]
#define CALLERROR  4
typedef unsigned char OCPPMessageType;


typedef struct
{
    const unsigned int messageID;
    const char         action[30];
    const char         payload[256]; // JSON

} OCPPCall;

typedef struct
{
    const unsigned int messageID;
    const char         payload[256];
} OCPPCallResult;

typedef struct
{
    const unsigned int messageID;
    const char         error_code[30];
    const char         error_dscr[255];
    const char         error_details[256];
} OCPPCallError;

typedef union
{
    OCPPCall       call;
    OCPPCallResult call_result;
    OCPPCallError  call_error;
} OCPPMessage;



#endif /* OCPP_TYPES_H_ */