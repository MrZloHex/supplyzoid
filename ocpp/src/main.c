#include <stdio.h>

#include "OCPP.h"

int
main()
{
    OCPPCall call_cfg = {
        .messageID = 19223201,
        .action    = "BootNotification",
        .payload   = "{\"reason\":\"PowerUp\",\"chargingStation\":{\"model\":\"SingleSocketCharger\",\"vendorName\":\"VendorX\"}}"
    };

    // char *call = (char*)malloc(sizeof(char)*1024);
    char call[1024];
    make_call(call_cfg, call);

    printf("NEW CALL: `%s`\n", call);

    // free(call);


    char call_result[] = "[3,\"19223201\",{\"currentTime\":\"2013-02-01T20:53:32.486Z\",\"interval\":300,\"status\":\"Accepted\"}]";

    OCPPResponse response_r;
    int error_r = handle_response(call_result, &response_r);
    if      (error_r == CALLERROR)
        printf("GOT CALLERROR\n");
    else if (error_r == CALLRESULT)
        printf("GOT CALLRESULT\n");


    char call_error[] = "[4,\"19223201\",\"ProtocolError\",\"Payload for Action is not conform the PDU structure\",{}]";

    OCPPResponse response_e;
    int error_e = handle_response(call_error, &response_e);
    if      (error_e == CALLERROR)
        printf("GOT CALLERROR\n");
    else if (error_e == CALLRESULT)
        printf("GOT CALLRESULT\n");

    return 0;
}

