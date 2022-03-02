#include <Arduino.h>
#include <unity.h>

#include "string_tests.h"
#include "rapi_tests.h"
#include "math_tests.h"
#include "convert_tests.h"


void test_equal(void) {
	TEST_ASSERT_EQUAL(32, 32);
}

void setup() {
	// NOTE!!! Wait for >2 secs
	// if board doesn't support software reset via Serial.DTR/RTS
	delay(2000);
	
	UNITY_BEGIN();
	RUN_TEST(test_equal);
	
	RUN_TEST(strcpy_test);
	RUN_TEST(strncpy_test);
	RUN_TEST(strlen_test);
	RUN_TEST(strcmp_test);
	RUN_TEST(memset_test);

	RUN_TEST(upower_test);
	
	RUN_TEST(int_to_charset_test);
	RUN_TEST(str_to_num_test);
	RUN_TEST(hex_str_to_u8_test);
	RUN_TEST(u8_to_hex_str_test);

	RUN_TEST(rapi_msg_correctness_test);
	RUN_TEST(rapi_chksum_test);
	UNITY_END();
}

void loop() {

}
