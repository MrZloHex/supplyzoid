#include <stdio.h>

#include "OCPP.h"

int
main()
{
    // TEST CALL REQUEST
    char call[] = "[2,\"19223201\",\"BootNotification\",{\"reason\":\"PowerUp\",\"chargingStation\":{\"model\":\"SingleSocketCharger\",\"vendorName\":\"VendorX\"}}]";
    // printf("%s\n", call);

    OCPPConfig config = {
        .smth = 69
    };

    result res = handle_call(&config, call);
    if (res != 0)
    {
        fprintf(stderr, "FAILED TO HANDLE CALL\n");
        return 1;
    }

    return 0;
}