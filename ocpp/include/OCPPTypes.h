#ifndef OCPP_TYPES_H_

#define OCPP_TYPES_H_

#define ERROR 0
typedef unsigned char OCPPResult;


#define ACTION_LEN   30
#define PAYLOAD_LEN  256
#define DSCR_LEN     255
#define ERR_CODE_LEN 30


#define POS_MSG_TYPE "$[0]"
#define POS_MSG_ID   "$[1]"

typedef unsigned char OCPPMessageType;
typedef unsigned long OCPPMessageID;



// [2,"19223201","BootNotification",{"reason":"PowerUp","chargingStation":{"model":"SingleSocketCharger","vendorName":"VendorX"}}]
#define CALL       2

#define POS_CL_ACT     "$[2]"
#define POS_CL_PAYLOAD "$[3]"
typedef unsigned char OCPPCallAction;

typedef struct
{
	OCPPCallAction action;
	char          *payload; // JSON

} OCPPCall;




// [3,"19223201",{"currentTime":"2013-02-01T20:53:32.486Z","interval":300,"status":"Accepted"}]
#define CALLRESULT 3

#define POS_CR_PAYLOAD "$[2]"

typedef struct
{
	char        *payload;
} OCPPCallResult;










// [4,"19223201","GenericError","Any other error not covered by the previous ones",{}]
#define CALLERROR  4

typedef struct
{
	char        *error_code;
	char        *error_dscr;
	char        *error_details;
} OCPPCallError;







typedef struct
{
	OCPPMessageType type;
	OCPPMessageID   ID;
	OCPPCall       	call;
	OCPPCallResult 	call_result;
	OCPPCallError  	call_error;
} OCPPMessage;



#endif /* OCPP_TYPES_H_ */