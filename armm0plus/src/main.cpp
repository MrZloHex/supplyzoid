#include <Arduino.h>

#ifdef __cplusplus
extern "C"
{
#endif
	#include "OCPP.h"
	#include "STM32RTC.h"
	#include "RAPI.h"
#ifdef __cplusplus
}
#endif

#include <HardwareSerial.h>



HardwareSerial serial(PA3, PA2);
HardwareSerial usart_rapi(PC5, PC4);

STM32RTC rtc;
RAPI rapi;

OCPP ocpp;

void
setup()
{
	stm32rtc_init(&rtc);
	stm32rtc_begin(&rtc, HOUR_FORMAT_24);

	serial.begin(9600);

	serial.printf("BOOTING UP\n");

	ocpp_init(&ocpp);
	rapi_reset(&rapi);
}

void
loop()
{
	ocpp_update(&ocpp, &rapi, &rtc);
	rapi_update(&rapi);
}
