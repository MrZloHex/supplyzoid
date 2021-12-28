#include <Arduino.h>

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