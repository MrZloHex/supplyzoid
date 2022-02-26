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