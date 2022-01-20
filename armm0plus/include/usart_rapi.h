#ifndef USART_RAPI_H_
#define USART_RAPI_H_

int
usart_rapi_available();

int
usart_rapi_read();

int
usart_rapi_print_str();

int
usart_rapi_print_ch(const char ch);

int
usart_rapi_print_u8(const u8 num);

int
usart_rapi_println_str();

#endif /* USART_RAPI_H_ */