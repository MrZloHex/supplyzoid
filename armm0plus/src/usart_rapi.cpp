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

extern "C"
int
usart_rapi_print_str(const char *str)
{
    return usart_rapi.printf("%s", str);
}

extern "C"
int
usart_rapi_print_ch(const char ch)
{
    return usart_rapi.printf("%c", ch);
}

extern "C"
int
usart_rapi_print_u8(const unsigned char num)
{
    return usart_rapi.printf("%u", num);
}

extern "C"
int
usart_rapi_println_str(const char *str)
{
    return usart_rapi.printf("%s\n", str);
}