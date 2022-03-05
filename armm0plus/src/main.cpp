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



// HardwareSerial serial(PA3, PA2);
// HardwareSerial usart_rapi(PC5, PC4);
HardwareSerial usart_rapi(PA3, PA2);
HardwareSerial usart_ocpp(PC5, PC4);


STM32RTC rtc;
RAPI rapi;
OCPP ocpp;

#ifndef UNIT_TEST
void
setup()
{
	stm32rtc_init(&rtc);
	stm32rtc_begin(&rtc, HOUR_FORMAT_24);

	usart_ocpp.begin(9600);
	usart_rapi.begin(9600);

	usart_rapi.printf("STARTING RAPI\n");
	usart_ocpp.printf("STARTING OCPP\n");

	ocpp_init(&ocpp);
	rapi_reset(&rapi);
}

void
loop()
{
	ocpp_update(&ocpp, &rapi, &rtc);
	rapi_update(&rapi, &ocpp);
}
#endif