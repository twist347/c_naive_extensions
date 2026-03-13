#include "unity.h"

#include "nx/core/optional.h"

void setUp(void) {
}

void tearDown(void) {
}

/* ========== basic some/none ========== */

static void test_opt_some(void) {
    nx_OptI32 o = NX_OPT_SOME(nx_OptI32, 42);
    TEST_ASSERT_TRUE(NX_OPT_HAS_VAL(o));
    TEST_ASSERT_FALSE(NX_OPT_IS_NONE(o));
    TEST_ASSERT_EQUAL_INT32(42, NX_OPT_UNWRAP(o));
}

static void test_opt_none(void) {
    nx_OptI32 o = NX_OPT_NONE(nx_OptI32);
    TEST_ASSERT_FALSE(NX_OPT_HAS_VAL(o));
    TEST_ASSERT_TRUE(NX_OPT_IS_NONE(o));
}

/* ========== unwrap_or ========== */

static void test_opt_unwrap_or_some(void) {
    nx_OptI32 o = NX_OPT_SOME(nx_OptI32, 10);
    TEST_ASSERT_EQUAL_INT32(10, NX_OPT_UNWRAP_OR(o, -1));
}

static void test_opt_unwrap_or_none(void) {
    nx_OptI32 o = NX_OPT_NONE(nx_OptI32);
    TEST_ASSERT_EQUAL_INT32(-1, NX_OPT_UNWRAP_OR(o, -1));
}

/* ========== unwrap_unchecked ========== */

static void test_opt_unwrap_unchecked(void) {
    nx_OptI32 o = NX_OPT_SOME(nx_OptI32, 42);
    TEST_ASSERT_EQUAL_INT32(42, NX_OPT_UNWRAP_UNCHECKED(o));
}

/* ========== various types ========== */

static void test_opt_usize(void) {
    nx_OptUsize o = NX_OPT_SOME(nx_OptUsize, 999);
    TEST_ASSERT_TRUE(NX_OPT_HAS_VAL(o));
    TEST_ASSERT_EQUAL_UINT64(999, NX_OPT_UNWRAP(o));
}

static void test_opt_bool(void) {
    nx_OptBool o = NX_OPT_SOME(nx_OptBool, false);
    TEST_ASSERT_TRUE(NX_OPT_HAS_VAL(o));
    TEST_ASSERT_FALSE(NX_OPT_UNWRAP(o));
}

static void test_opt_ptr_some(void) {
    nx_i32 x = 42;
    nx_OptPtr o = NX_OPT_SOME(nx_OptPtr, &x);
    TEST_ASSERT_TRUE(NX_OPT_HAS_VAL(o));
    TEST_ASSERT_EQUAL_PTR(&x, NX_OPT_UNWRAP(o));
}

static void test_opt_ptr_none(void) {
    nx_OptPtr o = NX_OPT_NONE(nx_OptPtr);
    TEST_ASSERT_TRUE(NX_OPT_IS_NONE(o));
    TEST_ASSERT_NULL(NX_OPT_UNWRAP_OR(o, nx_null));
}

/* ========== custom type ========== */

typedef struct {
    nx_i32 x;
    nx_i32 y;
} Vec2;

NX_DEF_OPT_TYPE(nx_OptVec2, Vec2);

static void test_opt_custom_type(void) {
    Vec2 v = {.x = 1, .y = 2};
    nx_OptVec2 o = NX_OPT_SOME(nx_OptVec2, v);
    TEST_ASSERT_TRUE(NX_OPT_HAS_VAL(o));

    Vec2 got = NX_OPT_UNWRAP(o);
    TEST_ASSERT_EQUAL_INT32(1, got.x);
    TEST_ASSERT_EQUAL_INT32(2, got.y);
}

static void test_opt_custom_type_none(void) {
    nx_OptVec2 o = NX_OPT_NONE(nx_OptVec2);
    Vec2 fallback = {.x = -1, .y = -1};
    Vec2 got = NX_OPT_UNWRAP_OR(o, fallback);
    TEST_ASSERT_EQUAL_INT32(-1, got.x);
    TEST_ASSERT_EQUAL_INT32(-1, got.y);
}

/* ========== zero is a valid value ========== */

static void test_opt_zero_is_some(void) {
    nx_OptI32 o = NX_OPT_SOME(nx_OptI32, 0);
    TEST_ASSERT_TRUE(NX_OPT_HAS_VAL(o));
    TEST_ASSERT_EQUAL_INT32(0, NX_OPT_UNWRAP(o));
}

static void test_opt_null_ptr_is_some(void) {
    // SOME(null) is NOT the same as NONE — the user explicitly put null in
    nx_OptPtr o = NX_OPT_SOME(nx_OptPtr, nx_null);
    TEST_ASSERT_TRUE(NX_OPT_HAS_VAL(o));
    TEST_ASSERT_NULL(NX_OPT_UNWRAP(o));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_opt_some);
    RUN_TEST(test_opt_none);
    RUN_TEST(test_opt_unwrap_or_some);
    RUN_TEST(test_opt_unwrap_or_none);
    RUN_TEST(test_opt_unwrap_unchecked);
    RUN_TEST(test_opt_usize);
    RUN_TEST(test_opt_bool);
    RUN_TEST(test_opt_ptr_some);
    RUN_TEST(test_opt_ptr_none);
    RUN_TEST(test_opt_custom_type);
    RUN_TEST(test_opt_custom_type_none);
    RUN_TEST(test_opt_zero_is_some);
    RUN_TEST(test_opt_null_ptr_is_some);

    return UNITY_END();
}
