#ifndef OCPP_TYPES_H_

#define OCPP_TYPES_H_

#define ERROR      0
// [2,"19223201","BootNotification",{"reason":"PowerUp","chargingStation":{"model":"SingleSocketCharger","vendorName":"VendorX"}}]
#define CALL       2
// [3,"19223201",{"currentTime":"2013-02-01T20:53:32.486Z","interval":300,"status":"Accepted"}]
#define CALLRESULT 3
// [4,"19223201","GenericError","Any other error not covered by the previous ones",{}]
#define CALLERROR  4
#define POS_MSG_TYPE "$[0]"
typedef unsigned char OCPPMessageType;

#define POS_MSG_ID   "$[1]"
typedef unsigned long OCPPMessageID;

#define ACTION_LEN   30
#define JSON_LEN     256
#define DSCR_LEN     255
#define ERR_CODE_LEN 30

typedef struct
{
	unsigned int messageID;
	char        *action;
	char        *payload; // JSON

} OCPPCall;

typedef struct
{
	unsigned int messageID;
	char        *payload;
} OCPPCallResult;

typedef struct
{
	unsigned int messageID;
	char        *error_code;
	char        *error_dscr;
	char        *error_details;
} OCPPCallError;

typedef struct
{
	OCPPCall       call;
	OCPPCallResult call_result;
	OCPPCallError  call_error;
} OCPPMessage;



#endif /* OCPP_TYPES_H_ */