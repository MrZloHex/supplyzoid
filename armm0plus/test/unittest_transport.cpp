#include "unittest_transport.h"

#include <Arduino.h>
#include <HardwareSerial.h>

static HardwareSerial unittest_serial(PA3, PA2);

extern "C"
void
unittest_uart_begin()
{
	unittest_serial.begin(115200);  
}

extern "C"
void
unittest_uart_putchar(char c)
{
	unittest_serial.printf("%c", c);
}

extern "C"
void
unittest_uart_flush()
{
	unittest_serial.flush();
}

extern "C"
void
unittest_uart_end()
{
	unittest_serial.end();
}
