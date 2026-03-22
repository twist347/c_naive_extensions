#include "unity.h"

#include "nx/algo/compare.h"
#include "nx/core/cmp.h"

void setUp(void) {
}

void tearDown(void) {
}

/* ========== equal ========== */

static void test_equal_same(void) {
    nx_i32 a[] = {1, 2, 3};
    nx_i32 b[] = {1, 2, 3};
    TEST_ASSERT_TRUE(nx_equal(NX_CSPAN_FROM_ARR(a), NX_CSPAN_FROM_ARR(b), nx_cmp_i32));
}

static void test_equal_different_values(void) {
    nx_i32 a[] = {1, 2, 3};
    nx_i32 b[] = {1, 2, 4};
    TEST_ASSERT_FALSE(nx_equal(NX_CSPAN_FROM_ARR(a), NX_CSPAN_FROM_ARR(b), nx_cmp_i32));
}

static void test_equal_different_len(void) {
    nx_i32 a[] = {1, 2, 3};
    nx_i32 b[] = {1, 2};
    TEST_ASSERT_FALSE(nx_equal(NX_CSPAN_FROM_ARR(a), NX_CSPAN_FROM_ARR(b), nx_cmp_i32));
}

static void test_equal_both_empty(void) {
    nx_CSpan a = nx_cspan_new(nx_null, 0, sizeof(nx_i32));
    nx_CSpan b = nx_cspan_new(nx_null, 0, sizeof(nx_i32));
    TEST_ASSERT_TRUE(nx_equal(a, b, nx_cmp_i32));
}

static void test_equal_one_empty(void) {
    nx_i32 arr[] = {1};
    nx_CSpan a = NX_CSPAN_FROM_ARR(arr);
    nx_CSpan b = nx_cspan_new(nx_null, 0, sizeof(nx_i32));
    TEST_ASSERT_FALSE(nx_equal(a, b, nx_cmp_i32));
}

/* ========== not_equal ========== */

static void test_not_equal_same(void) {
    nx_i32 a[] = {1, 2, 3};
    nx_i32 b[] = {1, 2, 3};
    TEST_ASSERT_FALSE(nx_not_equal(NX_CSPAN_FROM_ARR(a), NX_CSPAN_FROM_ARR(b), nx_cmp_i32));
}

static void test_not_equal_different(void) {
    nx_i32 a[] = {1, 2, 3};
    nx_i32 b[] = {1, 2, 4};
    TEST_ASSERT_TRUE(nx_not_equal(NX_CSPAN_FROM_ARR(a), NX_CSPAN_FROM_ARR(b), nx_cmp_i32));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_equal_same);
    RUN_TEST(test_equal_different_values);
    RUN_TEST(test_equal_different_len);
    RUN_TEST(test_equal_both_empty);
    RUN_TEST(test_equal_one_empty);
    RUN_TEST(test_not_equal_same);
    RUN_TEST(test_not_equal_different);

    return UNITY_END();
}
