#include "unity.h"

#include "nx/algo/sort.h"
#include "nx/core/util.h"

// TODO: impl

void setUp(void) {
}

void tearDown(void) {
}

// sort asc

static void test_nx_sort_asc_basic(void) {
    nx_i32 arr[] = {5, 1, 4, 2, 3, 0, -7, 9, 9};
    const nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_sort(s, nx_cmp_i32);
    TEST_ASSERT_TRUE(nx_is_sorted(nx_cspan_from_span(s), nx_cmp_i32));
}

static void test_nx_sort_asc_already_sorted(void) {
    nx_i32 arr[] = {-4, -1, -1, 0, 0, 3, 5, 5, 7};
    const nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_sort(s, nx_cmp_i32);
    TEST_ASSERT_TRUE(nx_is_sorted(nx_cspan_from_span(s), nx_cmp_i32));
}

// sort desc

static void test_nx_sort_desc_basic(void) {
    nx_i32 arr[] = {5, 1, 4, 2, 3, 0, -7, 9, 9};
    const nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_sort(s, nx_cmp_desc_i32);
    TEST_ASSERT_TRUE(nx_is_sorted(nx_cspan_from_span(s), nx_cmp_desc_i32));
}

static void test_nx_sort_desc_already_sorted(void) {
    nx_i32 arr[] = {-4, -1, -1, 0, 0, 3, 5, 5, 7};
    const nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_sort(s, nx_cmp_desc_i32);
    TEST_ASSERT_TRUE(nx_is_sorted(nx_cspan_from_span(s), nx_cmp_desc_i32));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_nx_sort_asc_basic);
    RUN_TEST(test_nx_sort_asc_already_sorted);

    RUN_TEST(test_nx_sort_desc_basic);
    RUN_TEST(test_nx_sort_desc_already_sorted);

    return UNITY_END();
}
