#ifndef SERIAL_H_
#define SERIAL_H_


int
serial_print_str(const char *str);

int
serial_println_str(const char *str);

int
serial_println_num(const int num);

bool
serial_available();

char
serial_read();

#endif /* SERIAL_H_ */