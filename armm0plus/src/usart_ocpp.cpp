#include <HardwareSerial.h>

extern HardwareSerial usart_ocpp;


extern "C"
int
serial_print_str(const char *str)
{
	return usart_ocpp.printf("%s", str);
}

extern "C"
int
serial_println_str(const char *str)
{
	return usart_ocpp.printf("%s\n", str);
}

extern "C"
int
serial_println_num(const int num)
{
	return usart_ocpp.printf("%d\n", num);
}


extern "C"
int
serial_available()
{
	return usart_ocpp.available();
}

extern "C"
char
serial_read()
{
	return (char)usart_ocpp.read();
}
