#include <Arduino.h>
#include <unity.h>

#include "string_tests.h"
#include "rapi_tests.h"


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
    RUN_TEST(rapi_analyze_test);
    RUN_TEST(rapi_chksum_test);
    UNITY_END();
}

void loop() {

}