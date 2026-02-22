#include "unity.h"

#include "nx/numeric/sat.h"
#include "nx/numeric/limit.h"

// TODO: add all cases

void setUp(void) {
}

void tearDown(void) {
}

void test_sat_add_i8_regular(void) {
    TEST_ASSERT_EQUAL_INT8(5, nx_sat_add_i8(2, 3));
    TEST_ASSERT_EQUAL_INT8(-5, nx_sat_add_i8(-2, -3));
    TEST_ASSERT_EQUAL_INT8(0, nx_sat_add_i8(-3, 3));
    TEST_ASSERT_EQUAL_INT8(0, nx_sat_add_i8(0, 0));
    TEST_ASSERT_EQUAL_INT8(1, nx_sat_add_i8(0, 1));
}

void test_sat_add_i8_sat(void) {
    TEST_ASSERT_EQUAL_INT8(NX_I8_MAX, nx_sat_add_i8(NX_I8_MAX, 1));
    TEST_ASSERT_EQUAL_INT8(NX_I8_MAX, nx_sat_add_i8(NX_I8_MAX, NX_I8_MAX));
    TEST_ASSERT_EQUAL_INT8(NX_I8_MAX, nx_sat_add_i8(100, 100));
    TEST_ASSERT_EQUAL_INT8(NX_I8_MIN, nx_sat_add_i8(NX_I8_MIN, -1));
    TEST_ASSERT_EQUAL_INT8(NX_I8_MIN, nx_sat_add_i8(NX_I8_MIN, NX_I8_MIN));
    TEST_ASSERT_EQUAL_INT8(NX_I8_MIN, nx_sat_add_i8(-100, -100));
}

int main() {
    UNITY_BEGIN();

    RUN_TEST(test_sat_add_i8_regular);
    RUN_TEST(test_sat_add_i8_sat);

    return UNITY_END();
}