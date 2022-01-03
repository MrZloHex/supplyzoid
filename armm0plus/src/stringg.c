#include "stringg.h"


void
strcpyy
(
	char       *_dest,
	const char *_src
)
{
	size i = 0;
	while (_src[i] != 0)
	{
		_dest[i] = _src[i];
		++i;
	}
	_dest[i] = '\0';
}

void
strncpyy
(
	char       *_dest,
	const char *_src,
	const size len
)
{
	for (size i = 0; i <= len; ++i)
	{
		if (i == len)
			_dest[i] = '\0';
		else
			_dest[i] = _src[i];
	}
}

size
strlenn
(
	const char *_str
)
{
	size len = 0;

	while (_str[len] != 0)
	{
		++len;
	}

	return len;
}

bool
strcmpp
(
	const char *str1,
	const char *str2
)
{
	if (strlenn(str1) != strlenn(str2))
		return false;

	size i = 0;
	bool res = true;
	while (str1[i] != 0)
	{
		if (str1[i] != str2[i])
		{
			res = false;
			break;
		}
		++i;
	}
	return res;
}

void
memsett
(
	char *_str,
	const int value,
	const size len
)
{
	for (size i = 0; i < len; ++i)
	{
		_str[i] = value;
	}
}