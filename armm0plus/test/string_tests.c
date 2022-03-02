#include "string_tests.h"

#include <unity.h>
#include <stringg.h>

void
strcpy_test(void)
{
	char oc[] = "OCPP";
	char ocpp[5];
	strcpyy(ocpp, oc);
	TEST_ASSERT_EQUAL_STRING("OCPP", ocpp);
}

void
strncpy_test(void)
{
	char oc[] = "OCPPQWE";
	char ocpp[5];
	strncpyy(ocpp, oc, 4);
	TEST_ASSERT_EQUAL_STRING("OCPP", ocpp);
}

void
strlen_test(void)
{
	char str[] = "TEST STRING";
	TEST_ASSERT_EQUAL(11, strlenn(str));
}

void
strcmp_test(void)
{
	char str1[] = "QWE";
	char str2[] = "QWE";
	TEST_ASSERT(strcmpp(str1, str2));
}

void
memset_test(void)
{
	char str[] = "QWE";
	memsett(str, '0', strlenn(str));
	TEST_ASSERT_EQUAL_STRING("000", str);
}

