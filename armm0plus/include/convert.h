#ifndef CONVERT_H_

#define CONVERT_H_

#include "stdtypes.h"
#include "mathh.h"
#include "stringg.h"

int
int_to_charset
(
	int  value,
	char set[],
	int  digits
);

#define STR_TO_NUM(T, S) _Generic((T), \
	u8:  charset_to_u8,                \
	u32: charset_to_u32,               \
	u64: charset_to_u64,               \
	default: charset_to_u32            \
	)(&T, S)


void
charset_to_u64
(
	u64 *dst,
	char set[]
);

void
charset_to_u32
(
	u32 *dst,
	char set[]
);

void
charset_to_u8
(
	u8 *dst,
	char set[]
);

void
hex_str_to_u8
(
	const char *str,
	u8 *dst
);

void
u8_to_hex_str
(
	char *str,
	u8 num
);

void
reverse
(
	char *set,
	int len
);

#endif /* CONVERT_H_ */