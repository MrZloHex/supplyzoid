#include "convert.h"

int
int_to_charset
(
	int  value,
	char set[],
	int  digits
)
{
	int i = 0;
	while (value)
	{
		set[i++] = (value % 10) + '0';
		value /= 10;
	}

	while (i < digits)
	{
		set[i++] = '0';
	}

	reverse(set, i);
	set[i] = '\0';
	return i;
}

void
charset_to_u64
(
	u64 *dst,
	char set[]
)
{
	size i = 0;
	size len = strlenn(set);
	unsigned long res = 0;
	while (set[i] != 0)
	{
		res += (set[i] - '0') * upower(10, (len-i-1));
		++i;
	}
	*dst = res;
}

void
charset_to_u32
(
	u32 *dst,
	char set[]
)
{
	size i = 0;
	size len = strlenn(set);
	unsigned long res = 0;
	while (set[i] != 0)
	{
		res += (set[i] - '0') * upower(10, (len-i-1));
		++i;
	}
	*dst = res;
}

void
charset_to_u8
(
	u8 *dst,
	char set[]
)
{
	size i = 0;
	size len = strlenn(set);
	unsigned long res = 0;
	while (set[i] != 0)
	{
		res += (set[i] - '0') * upower(10, (len-i-1));
		++i;
	}
	*dst = res;
}

void
hex_str_to_u8
(
	const char *str,
	u8 *dst
)
{
	u8 num = 0;
	for (size i = 0; i < 2; ++i)
	{
		char ch = str[i];
		if (ch != '\0')
		{
			if (i == 1)
				num <<= 4;
			if ((ch >= '0') && (ch <= '9'))
			{
				num += ch - '0';
			}
			else if ((ch >= 'A') && (ch <= 'F'))
			{
				num += ch - 'A' + 10;
			}
			else if ((ch >= 'a') && (ch <= 'f'))
			{
				num += ch - 'a' + 10;
			}
		}
	}
	*dst = num;
}

void
reverse
(
	char *set,
	int len
)
{
	int i = 0;
	int j = len - 1;
	int temp;
	
	while (i < j)
	{
		temp = set[i];
		set[i] = set[j];
		set[j] = temp;
		
		i++;
		j--;
	}
}
