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

void
charset_to_ulong
(
	unsigned long *dst,
	char set[]
);


void
reverse
(
	char *set,
	int len
);

#endif /* CONVERT_H_ */