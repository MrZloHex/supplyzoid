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