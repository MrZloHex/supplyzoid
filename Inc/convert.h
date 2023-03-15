#ifndef CONVERT_H_
#define CONVERT_H_

#include "stdint.h"


int
int_to_charset
(
	int  value,
	char set[],
	int  digits
);

#define STR_TO_NUM(T, S) _Generic((T), \
	uint8_t:  charset_to_uint8,        \
	uint32_t: charset_to_uint32,       \
	uint64_t: charset_to_uint64,       \
	default:  charset_to_uint32        \
	)(&T, S)


void
charset_to_uint64
(
	uint64_t *dst,
	char set[]
);

void
charset_to_uint32
(
	uint32_t *dst,
	char set[]
);

void
charset_to_uint8
(
	uint8_t *dst,
	char set[]
);

void
hex_str_to_uint8
(
	uint8_t *dst,
	const char *str
);

void
uint8_to_hex_str
(
	uint8_t num,
	char *str
);

void
reverse
(
	char *set,
	int len
);

unsigned long
upower
(
    int x,
    unsigned short y
);

#endif /* CONVERT_H_ */