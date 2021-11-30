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
}

void
loop()
{
	evse_update(&evse, &ocpp);
}


int
main()
{
	printf("[3,\"1\",{\"currentTime\":\"2013-02-01T20:53:32.486Z\",\"interval\":30,\"status\":\"Accepted\"}]\n");
	printf("[2,\"987\",\"RemoteStartTransaction\",{\"idTag\":\"hub\"}]\n");
	printf("[3,\"2\",{\"idTagInfo\":{\"status\":\"Accepted\"},\"transactionId\":420}]\n");

	setup();
	while(1)
	{
		loop();
	}

	ocpp_free(&ocpp);
	return 0;
}
