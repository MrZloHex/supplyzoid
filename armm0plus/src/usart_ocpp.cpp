#include <HardwareSerial.h>

extern HardwareSerial usart_ocpp;


extern "C"
int
usart_ocpp_print_str(const char *str)
{
	return usart_ocpp.printf("%s", str);
}

extern "C"
int
usart_ocpp_println_str(const char *str)
{
	return usart_ocpp.printf("%s\n", str);
}

extern "C"
int
usart_ocpp_println_num(const int num)
{
	return usart_ocpp.printf("%d\n", num);
}


extern "C"
int
usart_ocpp_available()
{
	return usart_ocpp.available();
}

extern "C"
char
usart_ocpp_read()
{
	return (char)usart_ocpp.read();
}
