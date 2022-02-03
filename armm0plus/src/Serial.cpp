#include <HardwareSerial.h>

extern HardwareSerial serial;


extern "C"
int
serial_print_str(const char *str)
{
	return serial.printf("%s", str);
}

extern "C"
int
serial_println_str(const char *str)
{
	return serial.printf("%s\n", str);
}

extern "C"
int
serial_println_num(const int num)
{
	return serial.printf("%d\n", num);
}


extern "C"
int
serial_available()
{
	return serial.available();
}

extern "C"
char
serial_read()
{
	return (char)serial.read();
}
