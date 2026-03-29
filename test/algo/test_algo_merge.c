#include "unity.h"

#include "nx/algo/merge.h"
#include "nx/core/cmp.h"

void setUp(void) {
}

void tearDown(void) {
}

/* ========== nx_merge ========== */

static void test_merge_basic(void) {
    nx_i32 a[] = {1, 3, 5, 7};
    nx_i32 b[] = {2, 4, 6, 8};
    nx_i32 dst[8] = {0};

    nx_merge(NX_SPAN_FROM_ARR(dst), NX_CSPAN_FROM_ARR(a), NX_CSPAN_FROM_ARR(b), nx_cmp_i32);

    const nx_i32 expected[] = {1, 2, 3, 4, 5, 6, 7, 8};
    TEST_ASSERT_EQUAL_MEMORY(expected, dst, sizeof(expected));
}

static void test_merge_left_empty(void) {
    nx_i32 b[] = {1, 2, 3};
    nx_i32 dst[3] = {0};

    nx_CSpan empty = nx_cspan_new(nx_null, 0, sizeof(nx_i32));
    nx_merge(NX_SPAN_FROM_ARR(dst), empty, NX_CSPAN_FROM_ARR(b), nx_cmp_i32);

    TEST_ASSERT_EQUAL_MEMORY(b, dst, sizeof(b));
}

static void test_merge_right_empty(void) {
    nx_i32 a[] = {1, 2, 3};
    nx_i32 dst[3] = {0};

    nx_CSpan empty = nx_cspan_new(nx_null, 0, sizeof(nx_i32));
    nx_merge(NX_SPAN_FROM_ARR(dst), NX_CSPAN_FROM_ARR(a), empty, nx_cmp_i32);

    TEST_ASSERT_EQUAL_MEMORY(a, dst, sizeof(a));
}

static void test_merge_both_empty(void) {
    nx_CSpan empty = nx_cspan_new(nx_null, 0, sizeof(nx_i32));
    nx_Span dst = nx_span_new(nx_null, 0, sizeof(nx_i32));

    nx_merge(dst, empty, empty, nx_cmp_i32); // must not crash
}

static void test_merge_interleaved(void) {
    nx_i32 a[] = {1, 2, 3};
    nx_i32 b[] = {1, 2, 3};
    nx_i32 dst[6] = {0};

    nx_merge(NX_SPAN_FROM_ARR(dst), NX_CSPAN_FROM_ARR(a), NX_CSPAN_FROM_ARR(b), nx_cmp_i32);

    const nx_i32 expected[] = {1, 1, 2, 2, 3, 3};
    TEST_ASSERT_EQUAL_MEMORY(expected, dst, sizeof(expected));
}

static void test_merge_no_overlap(void) {
    nx_i32 a[] = {1, 2, 3};
    nx_i32 b[] = {10, 20, 30};
    nx_i32 dst[6] = {0};

    nx_merge(NX_SPAN_FROM_ARR(dst), NX_CSPAN_FROM_ARR(a), NX_CSPAN_FROM_ARR(b), nx_cmp_i32);

    const nx_i32 expected[] = {1, 2, 3, 10, 20, 30};
    TEST_ASSERT_EQUAL_MEMORY(expected, dst, sizeof(expected));
}

static void test_merge_single_elements(void) {
    nx_i32 a[] = {5};
    nx_i32 b[] = {3};
    nx_i32 dst[2] = {0};

    nx_merge(NX_SPAN_FROM_ARR(dst), NX_CSPAN_FROM_ARR(a), NX_CSPAN_FROM_ARR(b), nx_cmp_i32);

    TEST_ASSERT_EQUAL_INT32(3, dst[0]);
    TEST_ASSERT_EQUAL_INT32(5, dst[1]);
}

static void test_merge_desc(void) {
    nx_i32 a[] = {7, 5, 3, 1};
    nx_i32 b[] = {8, 6, 4, 2};
    nx_i32 dst[8] = {0};

    nx_merge(NX_SPAN_FROM_ARR(dst), NX_CSPAN_FROM_ARR(a), NX_CSPAN_FROM_ARR(b), nx_cmp_desc_i32);

    const nx_i32 expected[] = {8, 7, 6, 5, 4, 3, 2, 1};
    TEST_ASSERT_EQUAL_MEMORY(expected, dst, sizeof(expected));
}

static void test_merge_unequal_lengths(void) {
    nx_i32 a[] = {1, 5, 9};
    nx_i32 b[] = {2, 3, 4, 6, 7, 8, 10};
    nx_i32 dst[10] = {0};

    nx_merge(NX_SPAN_FROM_ARR(dst), NX_CSPAN_FROM_ARR(a), NX_CSPAN_FROM_ARR(b), nx_cmp_i32);

    const nx_i32 expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    TEST_ASSERT_EQUAL_MEMORY(expected, dst, sizeof(expected));
}

/* ========== nx_inplace_merge ========== */

static void test_inplace_merge_basic(void) {
    nx_i32 arr[] = {1, 3, 5, 7, 2, 4, 6, 8};
    nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_inplace_merge(s, 4, nx_cmp_i32);

    const nx_i32 expected[] = {1, 2, 3, 4, 5, 6, 7, 8};
    TEST_ASSERT_EQUAL_MEMORY(expected, arr, sizeof(expected));
}

static void test_inplace_merge_mid_zero(void) {
    nx_i32 arr[] = {1, 2, 3};
    nx_i32 original[] = {1, 2, 3};
    nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_inplace_merge(s, 0, nx_cmp_i32);

    TEST_ASSERT_EQUAL_MEMORY(original, arr, sizeof(arr));
}

static void test_inplace_merge_mid_len(void) {
    nx_i32 arr[] = {1, 2, 3};
    nx_i32 original[] = {1, 2, 3};
    nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_inplace_merge(s, 3, nx_cmp_i32);

    TEST_ASSERT_EQUAL_MEMORY(original, arr, sizeof(arr));
}

static void test_inplace_merge_already_sorted(void) {
    nx_i32 arr[] = {1, 2, 3, 4, 5, 6};
    nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_inplace_merge(s, 3, nx_cmp_i32);

    const nx_i32 expected[] = {1, 2, 3, 4, 5, 6};
    TEST_ASSERT_EQUAL_MEMORY(expected, arr, sizeof(expected));
}

static void test_inplace_merge_reverse_halves(void) {
    nx_i32 arr[] = {4, 5, 6, 1, 2, 3};
    nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_inplace_merge(s, 3, nx_cmp_i32);

    const nx_i32 expected[] = {1, 2, 3, 4, 5, 6};
    TEST_ASSERT_EQUAL_MEMORY(expected, arr, sizeof(expected));
}

static void test_inplace_merge_unequal_halves(void) {
    nx_i32 arr[] = {2, 5, 8, 1, 3, 4, 6, 7, 9};
    nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_inplace_merge(s, 3, nx_cmp_i32);

    const nx_i32 expected[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    TEST_ASSERT_EQUAL_MEMORY(expected, arr, sizeof(expected));
}

static void test_inplace_merge_single_left(void) {
    nx_i32 arr[] = {5, 1, 2, 3, 4};
    nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_inplace_merge(s, 1, nx_cmp_i32);

    const nx_i32 expected[] = {1, 2, 3, 4, 5};
    TEST_ASSERT_EQUAL_MEMORY(expected, arr, sizeof(expected));
}

static void test_inplace_merge_single_right(void) {
    nx_i32 arr[] = {1, 2, 3, 4, 0};
    nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_inplace_merge(s, 4, nx_cmp_i32);

    const nx_i32 expected[] = {0, 1, 2, 3, 4};
    TEST_ASSERT_EQUAL_MEMORY(expected, arr, sizeof(expected));
}

static void test_inplace_merge_desc(void) {
    nx_i32 arr[] = {8, 6, 4, 2, 7, 5, 3, 1};
    nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_inplace_merge(s, 4, nx_cmp_desc_i32);

    const nx_i32 expected[] = {8, 7, 6, 5, 4, 3, 2, 1};
    TEST_ASSERT_EQUAL_MEMORY(expected, arr, sizeof(expected));
}

static void test_inplace_merge_with_duplicates(void) {
    nx_i32 arr[] = {1, 3, 3, 5, 2, 3, 4, 5};
    nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_inplace_merge(s, 4, nx_cmp_i32);

    const nx_i32 expected[] = {1, 2, 3, 3, 3, 4, 5, 5};
    TEST_ASSERT_EQUAL_MEMORY(expected, arr, sizeof(expected));
}

int main(void) {
    UNITY_BEGIN();

    /* merge */
    RUN_TEST(test_merge_basic);
    RUN_TEST(test_merge_left_empty);
    RUN_TEST(test_merge_right_empty);
    RUN_TEST(test_merge_both_empty);
    RUN_TEST(test_merge_interleaved);
    RUN_TEST(test_merge_no_overlap);
    RUN_TEST(test_merge_single_elements);
    RUN_TEST(test_merge_desc);
    RUN_TEST(test_merge_unequal_lengths);

    /* inplace_merge */
    RUN_TEST(test_inplace_merge_basic);
    RUN_TEST(test_inplace_merge_mid_zero);
    RUN_TEST(test_inplace_merge_mid_len);
    RUN_TEST(test_inplace_merge_already_sorted);
    RUN_TEST(test_inplace_merge_reverse_halves);
    RUN_TEST(test_inplace_merge_unequal_halves);
    RUN_TEST(test_inplace_merge_single_left);
    RUN_TEST(test_inplace_merge_single_right);
    RUN_TEST(test_inplace_merge_desc);
    RUN_TEST(test_inplace_merge_with_duplicates);

    return UNITY_END();
}
