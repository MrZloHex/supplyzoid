#ifndef STRING_H_

#define STRING_H_

#include "stdtypes.h"

void
strcpyy
(
	char       *_dest,
	const char *_src
);

void
strncpyy
(
	char       *_dest,
	const char *_src,
	const size len
);

size
strlenn
(
	const char *_str
);

bool
strcmpp
(
	const char *str1,
	const char *str2
);

void
memsett
(
	char *_str,
	const int value,
	const size len
);

#endif /* STRING_H_ */