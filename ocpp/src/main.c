#include <stdio.h>

#include "EVSE.h"
#include "OCPP.h"


OCPP ocpp;
EVSE evse;

void
setup()
{
	ocpp_init(&ocpp);
	evse_init(&evse);

	ocpp_send_req(&ocpp, &evse, BOOT_NOTIFICATION);
}

void
loop()
{
	ocpp_update(&ocpp, &evse);
}


int
main()
{

	setup();
	while(1)
	{
		loop();
	}

	ocpp_free(&ocpp);
	return 0;
}
