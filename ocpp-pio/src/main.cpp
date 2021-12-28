#include <Arduino.h>

#ifdef __cplusplus
extern "C"
{
#endif
	#include "OCPP.h"
	#include "EVSE.h"
#ifdef __cplusplus
}
#endif


#include <HardwareSerial.h>

HardwareSerial serial(PA3, PA2);

OCPP ocpp;
EVSE evse;

void
setup()
{
	serial.begin(9600);

	serial.printf("BOOTING UP\n");

	ocpp_init(&ocpp);
	evse_init(&evse);
}

void
loop()
{
	evse_update(&evse, &ocpp);
}