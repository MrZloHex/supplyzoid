#include "convert.h"

#include "math.h"
#include "string.h"

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
charset_to_uint64
(
	uint64_t *dst,
	char set[]
)
{
	size_t i = 0;
	size_t len = strlen(set);
	unsigned long res = 0;
	while (set[i] != 0)
	{
		res += (set[i] - '0') * upower(10, (len-i-1));
		++i;
	}
	*dst = res;
}

void
charset_to_uint32
(
	uint32_t *dst,
	char set[]
)
{
	size_t i = 0;
	size_t len = strlen(set);
	unsigned long res = 0;
	while (set[i] != 0)
	{
		res += (set[i] - '0') * upower(10, (len-i-1));
		++i;
	}
	*dst = res;
}

void
charset_to_uint8
(
	uint8_t *dst,
	char set[]
)
{
	size_t i = 0;
	size_t len = strlen(set);
	unsigned long res = 0;
	while (set[i] != 0)
	{
		res += (set[i] - '0') * upower(10, (len-i-1));
		++i;
	}
	*dst = res;
}

void
hex_str_to_uint8
(
	uint8_t *dst,
	const char *str
)
{
	uint8_t num = 0;
	// usart_rapi_println_str(str);
	for (size_t i = 0; i < 2; ++i)
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

void
uint8_to_hex_str
(
	uint8_t num,
	char *str
)
{
	for (size_t i = 0; i < 2; ++i)
	{
		uint8_t number = num;
		if (i == 0)
		{
			number >>= 4;
		}
		else if (i == 1)
		{
			number %= 16;
		}

		if (number < 10)
			str[i] = number + 48;
		else if (number < 16)
			str[i] = number + 55;
		
	}
}

unsigned long
upower
(
    int x,
    unsigned short y
)
{
    long res = 1;
    unsigned short exp = 0;
    while (exp < y)
    {
        res *= x;
        ++exp;
    }
    return res;
}
