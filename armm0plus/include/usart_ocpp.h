#ifndef USART_OCPP_H_
#define USART_OCPP_H_


int
usart_ocpp_print_str(const char *str);

int
usart_ocpp_println_str(const char *str);

int
usart_ocpp_println_num(const int num);

bool
usart_ocpp_available();

char
usart_ocpp_read();

#endif /* USART_OCPP_H_ */