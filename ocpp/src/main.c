#include <stdio.h>

#include "EVSE.h"
#include "OCPP.h"


EVSE evse;
OCPP ocpp;


void
setup()
{
	evse_init(&evse);
	ocpp_init(&ocpp);

	ocpp_send_req(&ocpp, BOOT_NOTIFICATION);
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
