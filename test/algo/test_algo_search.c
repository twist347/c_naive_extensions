#include "unity.h"

#include "nx/algo/search.h"
#include "nx/core/cmp.h"

void setUp(void) {
}

void tearDown(void) {
}

static nx_bool is_even(const void *elem) {
    return (*(const nx_i32 *) elem) % 2 == 0;
}

static nx_bool is_negative(const void *elem) {
    return *(const nx_i32 *) elem < 0;
}

/* ========== find ========== */

static void test_find_present(void) {
    nx_i32 arr[] = {10, 20, 30, 40, 50};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    nx_i32 key = 30;
    TEST_ASSERT_EQUAL_INT64(2, nx_find(s, &key, nx_cmp_i32));
}

static void test_find_absent(void) {
    nx_i32 arr[] = {10, 20, 30};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    nx_i32 key = 99;
    TEST_ASSERT_EQUAL_INT64(-1, nx_find(s, &key, nx_cmp_i32));
}

static void test_find_first_occurrence(void) {
    nx_i32 arr[] = {1, 2, 3, 2, 1};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    nx_i32 key = 2;
    TEST_ASSERT_EQUAL_INT64(1, nx_find(s, &key, nx_cmp_i32));
}

static void test_find_empty(void) {
    nx_CSpan s = nx_cspan_new(nx_null, 0, sizeof(nx_i32));
    nx_i32 key = 1;
    TEST_ASSERT_EQUAL_INT64(-1, nx_find(s, &key, nx_cmp_i32));
}

/* ========== contains ========== */

static void test_contains_true(void) {
    nx_i32 arr[] = {1, 2, 3};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    nx_i32 key = 2;
    TEST_ASSERT_TRUE(nx_contains(s, &key, nx_cmp_i32));
}

static void test_contains_false(void) {
    nx_i32 arr[] = {1, 2, 3};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    nx_i32 key = 99;
    TEST_ASSERT_FALSE(nx_contains(s, &key, nx_cmp_i32));
}

/* ========== count ========== */

static void test_count(void) {
    nx_i32 arr[] = {1, 2, 3, 2, 1, 2};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    nx_i32 key = 2;
    TEST_ASSERT_EQUAL_UINT64(3, nx_count(s, &key, nx_cmp_i32));
}

static void test_count_zero(void) {
    nx_i32 arr[] = {1, 2, 3};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    nx_i32 key = 99;
    TEST_ASSERT_EQUAL_UINT64(0, nx_count(s, &key, nx_cmp_i32));
}

/* ========== lower_bound (sorted) ========== */

static void test_lower_bound_present(void) {
    nx_i32 arr[] = {1, 2, 2, 3, 4, 4, 4, 5};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    nx_i32 key = 4;
    TEST_ASSERT_EQUAL_UINT64(4, nx_lower_bound(s, &key, nx_cmp_i32));
}

static void test_lower_bound_absent(void) {
    nx_i32 arr[] = {1, 3, 5, 7};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    nx_i32 key = 4;
    TEST_ASSERT_EQUAL_UINT64(2, nx_lower_bound(s, &key, nx_cmp_i32));
}

static void test_lower_bound_all_smaller(void) {
    nx_i32 arr[] = {1, 2, 3};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    nx_i32 key = 10;
    TEST_ASSERT_EQUAL_UINT64(3, nx_lower_bound(s, &key, nx_cmp_i32));
}

static void test_lower_bound_all_larger(void) {
    nx_i32 arr[] = {10, 20, 30};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    nx_i32 key = 1;
    TEST_ASSERT_EQUAL_UINT64(0, nx_lower_bound(s, &key, nx_cmp_i32));
}

/* ========== upper_bound (sorted) ========== */

static void test_upper_bound_present(void) {
    nx_i32 arr[] = {1, 2, 2, 3, 4, 4, 4, 5};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    nx_i32 key = 4;
    TEST_ASSERT_EQUAL_UINT64(7, nx_upper_bound(s, &key, nx_cmp_i32));
}

static void test_upper_bound_absent(void) {
    nx_i32 arr[] = {1, 3, 5, 7};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    nx_i32 key = 4;
    TEST_ASSERT_EQUAL_UINT64(2, nx_upper_bound(s, &key, nx_cmp_i32));
}

/* ========== bsearch (sorted) ========== */

static void test_bsearch_found(void) {
    nx_i32 arr[] = {1, 3, 5, 7, 9};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    nx_i32 key = 5;
    TEST_ASSERT_EQUAL_INT64(2, nx_bsearch(s, &key, nx_cmp_i32));
}

static void test_bsearch_not_found(void) {
    nx_i32 arr[] = {1, 3, 5, 7, 9};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    nx_i32 key = 4;
    TEST_ASSERT_EQUAL_INT64(-1, nx_bsearch(s, &key, nx_cmp_i32));
}

static void test_bsearch_first(void) {
    nx_i32 arr[] = {1, 3, 5};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    nx_i32 key = 1;
    TEST_ASSERT_EQUAL_INT64(0, nx_bsearch(s, &key, nx_cmp_i32));
}

static void test_bsearch_last(void) {
    nx_i32 arr[] = {1, 3, 5};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    nx_i32 key = 5;
    TEST_ASSERT_EQUAL_INT64(2, nx_bsearch(s, &key, nx_cmp_i32));
}

static void test_bsearch_empty(void) {
    nx_CSpan s = nx_cspan_new(nx_null, 0, sizeof(nx_i32));
    nx_i32 key = 1;
    TEST_ASSERT_EQUAL_INT64(-1, nx_bsearch(s, &key, nx_cmp_i32));
}

/* ========== find_if / count_if ========== */

static void test_find_if_found(void) {
    nx_i32 arr[] = {1, 3, 4, 7, 8};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    TEST_ASSERT_EQUAL_INT64(2, nx_find_if(s, is_even));
}

static void test_find_if_not_found(void) {
    nx_i32 arr[] = {1, 3, 5};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    TEST_ASSERT_EQUAL_INT64(-1, nx_find_if(s, is_even));
}

static void test_count_if(void) {
    nx_i32 arr[] = {1, 2, 3, 4, 5, 6};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    TEST_ASSERT_EQUAL_UINT64(3, nx_count_if(s, is_even));
}

static void test_count_if_none(void) {
    nx_i32 arr[] = {1, 2, 3};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    TEST_ASSERT_EQUAL_UINT64(0, nx_count_if(s, is_negative));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_find_present);
    RUN_TEST(test_find_absent);
    RUN_TEST(test_find_first_occurrence);
    RUN_TEST(test_find_empty);
    RUN_TEST(test_contains_true);
    RUN_TEST(test_contains_false);
    RUN_TEST(test_count);
    RUN_TEST(test_count_zero);
    RUN_TEST(test_lower_bound_present);
    RUN_TEST(test_lower_bound_absent);
    RUN_TEST(test_lower_bound_all_smaller);
    RUN_TEST(test_lower_bound_all_larger);
    RUN_TEST(test_upper_bound_present);
    RUN_TEST(test_upper_bound_absent);
    RUN_TEST(test_bsearch_found);
    RUN_TEST(test_bsearch_not_found);
    RUN_TEST(test_bsearch_first);
    RUN_TEST(test_bsearch_last);
    RUN_TEST(test_bsearch_empty);
    RUN_TEST(test_find_if_found);
    RUN_TEST(test_find_if_not_found);
    RUN_TEST(test_count_if);
    RUN_TEST(test_count_if_none);

    return UNITY_END();
}
