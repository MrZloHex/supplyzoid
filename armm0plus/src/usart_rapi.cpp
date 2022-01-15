#include "HardwareSerial.h"

extern HardwareSerial usart_rapi;

extern "C"
int
usart_rapi_available()
{
    return usart_rapi.available();
}

extern "C"
int
usart_rapi_read()
{
    return usart_rapi.read();
}
