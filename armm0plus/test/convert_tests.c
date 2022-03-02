#include <unity.h>
#include "convert_tests.h"

#include <convert.h>


void
int_to_charset_test(void)
{
	int num = 234;
	char str[4];
	int_to_charset(num, str, 0);
	TEST_ASSERT_EQUAL_STRING("234", str);
}

void
str_to_num_test(void)
{
	char str[] = "234";
	u32 num;
	STR_TO_NUM(num, str);
	TEST_ASSERT_EQUAL(234, num);
}

void
hex_str_to_u8_test(void)
{
	char str[] = "A0";
	u8 num;
	hex_str_to_u8(str, &num);
	TEST_ASSERT_EQUAL(160, num);
}

void
u8_to_hex_str_test(void)
{
	u8 num = 160;
	char str[3];
	u8_to_hex_str(str, num);
	TEST_ASSERT_EQUAL_STRING("A0", str);
}

