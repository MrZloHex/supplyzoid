#include <Arduino.h>

#ifdef __cplusplus
extern "C"
{
#endif
	#include "OCPP.h"
	#include "EVSE.h"
	#include "STM32RTC.h"
#ifdef __cplusplus
}
#endif

#include <HardwareSerial.h>



HardwareSerial serial(PA3, PA2);

STM32RTC rtc;

OCPP ocpp;
EVSE evse;

void
setup()
{
	stm32rtc_init(&rtc);
	stm32rtc_begin(&rtc, HOUR_FORMAT_24);

	serial.begin(9600);

	serial.printf("BOOTING UP\n");

	ocpp_init(&ocpp);
	evse_init(&evse);
}

void
loop()
{
	evse_update(&evse, &ocpp, &rtc);
}