// #include <stdio.h>

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

    // printf("%s\n", call);

    // free(call);


    char call_resultp[] = "[3,\"19223201\",{\"currentTime\":\"2013-02-01T20:53:32.486Z\",\"interval\":300,\"status\":\"Accepted\"}]";

    // handle_response();
    return 0;
}