#include "rapi_tests.h"

#include <unity.h>
#include "RAPI_Types.h"
#include "RAPI_msg_ingt.h"

void
rapi_msg_correctness_test(void)
{
	RAPI rapi = {
		.buf_index = 16,
		.token_index = 0,
		.buf_cmd = "$AB 00 D7.1.6^53"
	};
	TEST_ASSERT(rapi_is_msg_correct(&rapi));
}

void
rapi_chksum_test(void)
{
	RAPI rapi = {
		.buf_index = 14,
		.token_index = 0,
		.buf_cmd = "$AB 00 D7.1.6^"
	};
	rapi_append_chksum(&rapi);
	TEST_ASSERT_EQUAL_STRING("$AB 00 D7.1.6^53\r", rapi.buf_cmd);
}
