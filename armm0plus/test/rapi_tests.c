#include "rapi_tests.h"

#include <unity.h>
#include "RAPI_Types.h"
#include "RAPI_add.h"

void
rapi_analyze_test(void)
{
    RAPI rapi = {
        .buf_index = 0,
        .token_index = 0,
        .buf_cmd = "$AB 00 D7.1.6^53"
    };
    TEST_ASSERT(rapi_analyze(&rapi));
}

void
rapi_chksum_test(void)
{
    TEST_ASSERT(1);
}