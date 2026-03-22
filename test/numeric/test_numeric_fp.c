#include "unity.h"

#include "nx/numeric/fp.h"
#include "nx/core/limits.h"

void setUp(void) {
}

void tearDown(void) {
}

/* ========== lerp ========== */

static void test_lerp_f64_boundaries(void) {
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, 0.0, nx_lerp_f64(0.0, 10.0, 0.0));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, 10.0, nx_lerp_f64(0.0, 10.0, 1.0));
}

static void test_lerp_f64_midpoint(void) {
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, 5.0, nx_lerp_f64(0.0, 10.0, 0.5));
}

static void test_lerp_f64_extrapolate(void) {
    // t outside [0,1] extrapolates
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, -10.0, nx_lerp_f64(0.0, 10.0, -1.0));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, 20.0, nx_lerp_f64(0.0, 10.0, 2.0));
}

static void test_lerp_f64_negative(void) {
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, -5.0, nx_lerp_f64(-10.0, 0.0, 0.5));
}

static void test_lerp_f32(void) {
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 5.0f, nx_lerp_f32(0.0f, 10.0f, 0.5f));
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 0.0f, nx_lerp_f32(0.0f, 10.0f, 0.0f));
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 10.0f, nx_lerp_f32(0.0f, 10.0f, 1.0f));
}

static void test_lerp_generic(void) {
    const nx_f64 a = 0.0, b = 100.0, t = 0.25;
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, 25.0, nx_lerp(a, b, t));
}

/* ========== inv_lerp ========== */

static void test_inv_lerp_f64(void) {
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, 0.0, nx_inv_lerp_f64(0.0, 10.0, 0.0));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, 0.5, nx_inv_lerp_f64(0.0, 10.0, 5.0));
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, 1.0, nx_inv_lerp_f64(0.0, 10.0, 10.0));
}

static void test_inv_lerp_f32(void) {
    TEST_ASSERT_FLOAT_WITHIN(1e-5f, 0.5f, nx_inv_lerp_f32(0.0f, 10.0f, 5.0f));
}

static void test_lerp_inv_lerp_roundtrip(void) {
    const nx_f64 a = 3.0, b = 17.0, val = 10.0;
    const nx_f64 t = nx_inv_lerp_f64(a, b, val);
    const nx_f64 res = nx_lerp_f64(a, b, t);
    TEST_ASSERT_DOUBLE_WITHIN(1e-12, val, res);
}

/* ========== approx_eq ========== */

static void test_approx_eq_f64_equal(void) {
    TEST_ASSERT_TRUE(nx_approx_eq_f64(1.0, 1.0, 0.0));
    TEST_ASSERT_TRUE(nx_approx_eq_f64(1.0, 1.0 + 1e-15, 1e-14));
}

static void test_approx_eq_f64_not_equal(void) {
    TEST_ASSERT_FALSE(nx_approx_eq_f64(1.0, 2.0, 0.5));
}

static void test_approx_eq_f64_boundary(void) {
    // exactly at eps
    TEST_ASSERT_TRUE(nx_approx_eq_f64(0.0, 0.1, 0.1));
    TEST_ASSERT_FALSE(nx_approx_eq_f64(0.0, 0.1 + 1e-15, 0.1));
}

static void test_approx_eq_f32(void) {
    TEST_ASSERT_TRUE(nx_approx_eq_f32(1.0f, 1.001f, 0.01f));
    TEST_ASSERT_FALSE(nx_approx_eq_f32(1.0f, 1.1f, 0.01f));
}

static void test_approx_eq_generic(void) {
    const nx_f64 a = 3.14, b = 3.14159;
    TEST_ASSERT_TRUE(nx_approx_eq(a, b, 0.01));
    TEST_ASSERT_FALSE(nx_approx_eq(a, b, 0.0001));
}

int main(void) {
    UNITY_BEGIN();

    // lerp
    RUN_TEST(test_lerp_f64_boundaries);
    RUN_TEST(test_lerp_f64_midpoint);
    RUN_TEST(test_lerp_f64_extrapolate);
    RUN_TEST(test_lerp_f64_negative);
    RUN_TEST(test_lerp_f32);
    RUN_TEST(test_lerp_generic);

    // inv_lerp
    RUN_TEST(test_inv_lerp_f64);
    RUN_TEST(test_inv_lerp_f32);
    RUN_TEST(test_lerp_inv_lerp_roundtrip);

    // approx_eq
    RUN_TEST(test_approx_eq_f64_equal);
    RUN_TEST(test_approx_eq_f64_not_equal);
    RUN_TEST(test_approx_eq_f64_boundary);
    RUN_TEST(test_approx_eq_f32);
    RUN_TEST(test_approx_eq_generic);

    return UNITY_END();
}
