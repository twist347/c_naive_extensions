#include "unity.h"

#include "nx/algo/minmax.h"
#include "nx/core/cmp.h"

void setUp(void) {
}

void tearDown(void) {
}

/* ========== min_element ========== */

static void test_min_element_basic(void) {
    nx_i32 arr[] = {5, 3, 1, 4, 2};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    TEST_ASSERT_EQUAL_INT64(2, nx_min_element(s, nx_cmp_i32));
}

static void test_min_element_first(void) {
    nx_i32 arr[] = {1, 2, 3};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    TEST_ASSERT_EQUAL_INT64(0, nx_min_element(s, nx_cmp_i32));
}

static void test_min_element_last(void) {
    nx_i32 arr[] = {3, 2, 1};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    TEST_ASSERT_EQUAL_INT64(2, nx_min_element(s, nx_cmp_i32));
}

static void test_min_element_single(void) {
    nx_i32 val = 42;
    nx_CSpan s = NX_CSPAN_FROM_PTR(&val, 1);
    TEST_ASSERT_EQUAL_INT64(0, nx_min_element(s, nx_cmp_i32));
}

static void test_min_element_empty(void) {
    nx_CSpan s = nx_cspan_new(nx_null, 0, sizeof(nx_i32));
    TEST_ASSERT_EQUAL_INT64(-1, nx_min_element(s, nx_cmp_i32));
}

static void test_min_element_duplicates(void) {
    nx_i32 arr[] = {3, 1, 4, 1, 5};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    // should return first occurrence of min
    TEST_ASSERT_EQUAL_INT64(1, nx_min_element(s, nx_cmp_i32));
}

/* ========== max_element ========== */

static void test_max_element_basic(void) {
    nx_i32 arr[] = {5, 3, 1, 4, 2};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    TEST_ASSERT_EQUAL_INT64(0, nx_max_element(s, nx_cmp_i32));
}

static void test_max_element_last(void) {
    nx_i32 arr[] = {1, 2, 3};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    TEST_ASSERT_EQUAL_INT64(2, nx_max_element(s, nx_cmp_i32));
}

static void test_max_element_empty(void) {
    nx_CSpan s = nx_cspan_new(nx_null, 0, sizeof(nx_i32));
    TEST_ASSERT_EQUAL_INT64(-1, nx_max_element(s, nx_cmp_i32));
}

/* ========== minmax_element ========== */

static void test_minmax_element_basic(void) {
    nx_i32 arr[] = {5, 3, 1, 4, 2};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    nx_MinMax mm = nx_minmax_element(s, nx_cmp_i32);
    TEST_ASSERT_EQUAL_INT64(2, mm.min);
    TEST_ASSERT_EQUAL_INT64(0, mm.max);
}

static void test_minmax_element_single(void) {
    nx_i32 val = 42;
    nx_CSpan s = NX_CSPAN_FROM_PTR(&val, 1);
    nx_MinMax mm = nx_minmax_element(s, nx_cmp_i32);
    TEST_ASSERT_EQUAL_INT64(0, mm.min);
    TEST_ASSERT_EQUAL_INT64(0, mm.max);
}

static void test_minmax_element_empty(void) {
    nx_CSpan s = nx_cspan_new(nx_null, 0, sizeof(nx_i32));
    nx_MinMax mm = nx_minmax_element(s, nx_cmp_i32);
    TEST_ASSERT_EQUAL_INT64(-1, mm.min);
    TEST_ASSERT_EQUAL_INT64(-1, mm.max);
}

static void test_minmax_element_sorted(void) {
    nx_i32 arr[] = {1, 2, 3, 4, 5};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    nx_MinMax mm = nx_minmax_element(s, nx_cmp_i32);
    TEST_ASSERT_EQUAL_INT64(0, mm.min);
    TEST_ASSERT_EQUAL_INT64(4, mm.max);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_min_element_basic);
    RUN_TEST(test_min_element_first);
    RUN_TEST(test_min_element_last);
    RUN_TEST(test_min_element_single);
    RUN_TEST(test_min_element_empty);
    RUN_TEST(test_min_element_duplicates);

    RUN_TEST(test_max_element_basic);
    RUN_TEST(test_max_element_last);
    RUN_TEST(test_max_element_empty);

    RUN_TEST(test_minmax_element_basic);
    RUN_TEST(test_minmax_element_single);
    RUN_TEST(test_minmax_element_empty);
    RUN_TEST(test_minmax_element_sorted);

    return UNITY_END();
}
