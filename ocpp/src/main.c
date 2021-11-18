#include <stdio.h>

#include "OCPP.h"

int
main()
{
    // TEST CALL REQUEST
    char check_call[] = "[2,\"19223201\",\"BootNotification\",{\"reason\":\"PowerUp\",\"chargingStation\":{\"model\":\"SingleSocketCharger\",\"vendorName\":\"VendorX\"}}]";
    // printf("%s\n", call);

    OCPPCall call_cfg = {
        .messageID = 19223201,
        .action    = "BootNotification",
        .payload   = "{\"reason\":\"PowerUp\",\"chargingStation\":{\"model\":\"SingleSocketCharger\",\"vendorName\":\"VendorX\"}}"
    };

    char *call = (char*)malloc(sizeof(char)*1024);
    make_call(call_cfg, call);

    printf("%s\n", call);
        

    free(call);
    return 0;
}