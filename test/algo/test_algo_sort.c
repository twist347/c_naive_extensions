#include "unity.h"

#include <string.h>

#include "nx/algo/sort.h"
#include "nx/core/cmp.h"
#include "nx/core/util.h"

void setUp(void) {
}

void tearDown(void) {
}

/* ========== nx_sort ========== */

static void test_sort_basic(void) {
    nx_i32 arr[] = {5, 1, 4, 2, 3, 0, -7, 9, 9};
    const nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_sort(s, nx_cmp_i32);
    TEST_ASSERT_TRUE(nx_is_sorted(nx_cspan_from_span(s), nx_cmp_i32));
}

static void test_sort_already_sorted(void) {
    nx_i32 arr[] = {-4, -1, -1, 0, 0, 3, 5, 5, 7};
    const nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_sort(s, nx_cmp_i32);
    TEST_ASSERT_TRUE(nx_is_sorted(nx_cspan_from_span(s), nx_cmp_i32));
}

static void test_sort_desc(void) {
    nx_i32 arr[] = {5, 1, 4, 2, 3, 0, -7, 9, 9};
    const nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_sort(s, nx_cmp_desc_i32);
    TEST_ASSERT_TRUE(nx_is_sorted(nx_cspan_from_span(s), nx_cmp_desc_i32));
}

static void test_sort_empty(void) {
    nx_Span s = nx_span_new(nx_null, 0, sizeof(nx_i32));
    nx_sort(s, nx_cmp_i32); // must not crash
}

static void test_sort_single(void) {
    nx_i32 val = 42;
    nx_Span s = NX_SPAN_FROM_PTR(&val, 1);
    nx_sort(s, nx_cmp_i32);
    TEST_ASSERT_EQUAL_INT32(42, val);
}

static void test_sort_all_equal(void) {
    nx_i32 arr[] = {7, 7, 7, 7, 7};
    const nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_sort(s, nx_cmp_i32);
    for (int i = 0; i < 5; ++i) {
        TEST_ASSERT_EQUAL_INT32(7, arr[i]);
    }
}

static void test_sort_reverse(void) {
    nx_i32 arr[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    const nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_sort(s, nx_cmp_i32);
    for (int i = 0; i < 10; ++i) {
        TEST_ASSERT_EQUAL_INT32(i, arr[i]);
    }
}

/* ========== nx_sort_stable ========== */

static void test_stable_sort_basic(void) {
    nx_i32 arr[] = {5, 3, 1, 4, 2};
    const nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_sort_stable(s, nx_cmp_i32);
    TEST_ASSERT_TRUE(nx_is_sorted(nx_cspan_from_span(s), nx_cmp_i32));
}

static void test_stable_sort_empty(void) {
    nx_Span s = nx_span_new(nx_null, 0, sizeof(nx_i32));
    nx_sort_stable(s, nx_cmp_i32);
}

static void test_stable_sort_single(void) {
    nx_i32 val = 42;
    nx_Span s = NX_SPAN_FROM_PTR(&val, 1);
    nx_sort_stable(s, nx_cmp_i32);
    TEST_ASSERT_EQUAL_INT32(42, val);
}

static void test_stable_sort_already_sorted(void) {
    nx_i32 arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
    const nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_sort_stable(s, nx_cmp_i32);
    for (int i = 0; i < 8; ++i) {
        TEST_ASSERT_EQUAL_INT32(i + 1, arr[i]);
    }
}

static void test_stable_sort_reverse(void) {
    nx_i32 arr[] = {5, 4, 3, 2, 1};
    const nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_sort_stable(s, nx_cmp_i32);
    for (int i = 0; i < 5; ++i) {
        TEST_ASSERT_EQUAL_INT32(i + 1, arr[i]);
    }
}

static void test_stable_sort_all_equal(void) {
    nx_i32 arr[] = {3, 3, 3, 3, 3};
    const nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_sort_stable(s, nx_cmp_i32);
    for (int i = 0; i < 5; ++i) {
        TEST_ASSERT_EQUAL_INT32(3, arr[i]);
    }
}

static void test_stable_sort_desc(void) {
    nx_i32 arr[] = {1, 5, 3, 2, 4};
    const nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_sort_stable(s, nx_cmp_desc_i32);
    TEST_ASSERT_TRUE(nx_is_sorted(nx_cspan_from_span(s), nx_cmp_desc_i32));
}

/* ========== stability test ========== */

typedef struct {
    nx_i32 key;
    nx_i32 original_idx;
} Pair;

static int cmp_pair_by_key(const void *a, const void *b) {
    const Pair *pa = a;
    const Pair *pb = b;
    return (pa->key > pb->key) - (pa->key < pb->key);
}

static void test_stable_sort_preserves_order(void) {
    // multiple elements with same key — stable sort must preserve their relative order
    Pair arr[] = {
        {3, 0}, {1, 1}, {2, 2}, {1, 3}, {3, 4},
        {2, 5}, {1, 6}, {3, 7}, {2, 8}, {1, 9},
    };
    const nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_sort_stable(s, cmp_pair_by_key);

    // verify sorted by key
    for (nx_usize i = 1; i < 10; ++i) {
        TEST_ASSERT_TRUE(arr[i].key >= arr[i - 1].key);
    }

    // verify stability: within each group of equal keys,
    // original_idx must be strictly increasing
    for (nx_usize i = 1; i < 10; ++i) {
        if (arr[i].key == arr[i - 1].key) {
            TEST_ASSERT_TRUE(arr[i].original_idx > arr[i - 1].original_idx);
        }
    }
}

static void test_stable_sort_stability_large(void) {
    // larger test: 200 elements, keys in [0, 5)
    constexpr nx_usize N = 200;
    Pair arr[N];
    for (nx_usize i = 0; i < N; ++i) {
        arr[i].key = (nx_i32) (i % 5);
        arr[i].original_idx = (nx_i32) i;
    }

    const nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_sort_stable(s, cmp_pair_by_key);

    for (nx_usize i = 1; i < N; ++i) {
        TEST_ASSERT_TRUE(arr[i].key >= arr[i - 1].key);
        if (arr[i].key == arr[i - 1].key) {
            TEST_ASSERT_TRUE(arr[i].original_idx > arr[i - 1].original_idx);
        }
    }
}

/* ========== is_sorted ========== */

static void test_is_sorted_true(void) {
    nx_i32 arr[] = {1, 2, 3, 4, 5};
    TEST_ASSERT_TRUE(nx_is_sorted(NX_CSPAN_FROM_ARR(arr), nx_cmp_i32));
}

static void test_is_sorted_false(void) {
    nx_i32 arr[] = {1, 3, 2};
    TEST_ASSERT_FALSE(nx_is_sorted(NX_CSPAN_FROM_ARR(arr), nx_cmp_i32));
}

static void test_is_sorted_empty(void) {
    nx_CSpan s = nx_cspan_new(nx_null, 0, sizeof(nx_i32));
    TEST_ASSERT_TRUE(nx_is_sorted(s, nx_cmp_i32));
}

static void test_is_sorted_single(void) {
    nx_i32 val = 42;
    nx_CSpan s = NX_CSPAN_FROM_PTR(&val, 1);
    TEST_ASSERT_TRUE(nx_is_sorted(s, nx_cmp_i32));
}

static void test_is_sorted_with_dups(void) {
    nx_i32 arr[] = {1, 1, 2, 2, 3};
    TEST_ASSERT_TRUE(nx_is_sorted(NX_CSPAN_FROM_ARR(arr), nx_cmp_i32));
}

/* ========== partial_sort ========== */

static void test_partial_sort_basic(void) {
    nx_i32 arr[] = {9, 7, 5, 3, 1, 8, 6, 4, 2, 0};
    nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_partial_sort(s, 4, nx_cmp_i32);

    // first 4 must be the 4 smallest, sorted
    TEST_ASSERT_EQUAL_INT32(0, arr[0]);
    TEST_ASSERT_EQUAL_INT32(1, arr[1]);
    TEST_ASSERT_EQUAL_INT32(2, arr[2]);
    TEST_ASSERT_EQUAL_INT32(3, arr[3]);
}

static void test_partial_sort_k_equals_len(void) {
    nx_i32 arr[] = {5, 3, 1, 4, 2};
    nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_partial_sort(s, 5, nx_cmp_i32);

    // should fully sort
    TEST_ASSERT_TRUE(nx_is_sorted(nx_cspan_from_span(s), nx_cmp_i32));
}

static void test_partial_sort_k_zero(void) {
    nx_i32 arr[] = {5, 3, 1};
    nx_i32 original[] = {5, 3, 1};
    nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_partial_sort(s, 0, nx_cmp_i32);

    // nothing should change
    TEST_ASSERT_EQUAL_MEMORY(original, arr, sizeof(arr));
}

static void test_partial_sort_k_one(void) {
    nx_i32 arr[] = {5, 3, 1, 4, 2};
    nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_partial_sort(s, 1, nx_cmp_i32);

    // first element must be the smallest
    TEST_ASSERT_EQUAL_INT32(1, arr[0]);
}

static void test_partial_sort_desc(void) {
    nx_i32 arr[] = {1, 5, 3, 9, 7, 2, 8, 4, 6, 0};
    nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_partial_sort(s, 3, nx_cmp_desc_i32);

    // first 3 must be the 3 largest, sorted descending
    TEST_ASSERT_EQUAL_INT32(9, arr[0]);
    TEST_ASSERT_EQUAL_INT32(8, arr[1]);
    TEST_ASSERT_EQUAL_INT32(7, arr[2]);
}

static void test_partial_sort_empty(void) {
    nx_Span s = nx_span_new(nx_null, 0, sizeof(nx_i32));
    nx_partial_sort(s, 0, nx_cmp_i32); // must not crash
}

/* ========== nth_element ========== */

static void test_nth_element_median(void) {
    nx_i32 arr[] = {9, 7, 5, 3, 1, 8, 6, 4, 2, 0};
    nx_Span s = NX_SPAN_FROM_ARR(arr);

    // after nth_element(s, 5), arr[5] should be what would be at index 5 in sorted order = 5
    nx_nth_element(s, 5, nx_cmp_i32);
    TEST_ASSERT_EQUAL_INT32(5, arr[5]);

    // everything before index 5 should be <= arr[5]
    for (nx_usize i = 0; i < 5; ++i) {
        TEST_ASSERT_TRUE(arr[i] <= arr[5]);
    }

    // everything after index 5 should be >= arr[5]
    for (nx_usize i = 6; i < 10; ++i) {
        TEST_ASSERT_TRUE(arr[i] >= arr[5]);
    }
}

static void test_nth_element_first(void) {
    nx_i32 arr[] = {5, 3, 1, 4, 2};
    nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_nth_element(s, 0, nx_cmp_i32);
    TEST_ASSERT_EQUAL_INT32(1, arr[0]);
}

static void test_nth_element_last(void) {
    nx_i32 arr[] = {5, 3, 1, 4, 2};
    nx_Span s = NX_SPAN_FROM_ARR(arr);

    nx_nth_element(s, 4, nx_cmp_i32);
    TEST_ASSERT_EQUAL_INT32(5, arr[4]);
}

static void test_nth_element_single(void) {
    nx_i32 val = 42;
    nx_Span s = NX_SPAN_FROM_PTR(&val, 1);
    nx_nth_element(s, 0, nx_cmp_i32);
    TEST_ASSERT_EQUAL_INT32(42, val);
}

static void test_nth_element_all_equal(void) {
    nx_i32 arr[] = {7, 7, 7, 7, 7};
    nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_nth_element(s, 2, nx_cmp_i32);
    TEST_ASSERT_EQUAL_INT32(7, arr[2]);
}

static void test_nth_element_partitions_correctly(void) {
    // larger test
    nx_i32 arr[] = {20, 15, 10, 5, 0, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70};
    constexpr nx_usize len = 15;
    nx_Span s = NX_SPAN_FROM_ARR(arr);

    constexpr nx_usize nth = 7;
    nx_nth_element(s, nth, nx_cmp_i32);

    // the value at nth should be correct
    // sorted: 0,5,10,15,20,25,30,35,40,45,50,55,60,65,70 => arr[7] = 35
    TEST_ASSERT_EQUAL_INT32(35, arr[nth]);

    for (nx_usize i = 0; i < nth; ++i) {
        TEST_ASSERT_TRUE(arr[i] <= arr[nth]);
    }
    for (nx_usize i = nth + 1; i < len; ++i) {
        TEST_ASSERT_TRUE(arr[i] >= arr[nth]);
    }
}

int main(void) {
    UNITY_BEGIN();

    /* sort */
    RUN_TEST(test_sort_basic);
    RUN_TEST(test_sort_already_sorted);
    RUN_TEST(test_sort_desc);
    RUN_TEST(test_sort_empty);
    RUN_TEST(test_sort_single);
    RUN_TEST(test_sort_all_equal);
    RUN_TEST(test_sort_reverse);

    /* stable sort */
    RUN_TEST(test_stable_sort_basic);
    RUN_TEST(test_stable_sort_empty);
    RUN_TEST(test_stable_sort_single);
    RUN_TEST(test_stable_sort_already_sorted);
    RUN_TEST(test_stable_sort_reverse);
    RUN_TEST(test_stable_sort_all_equal);
    RUN_TEST(test_stable_sort_desc);
    RUN_TEST(test_stable_sort_preserves_order);
    RUN_TEST(test_stable_sort_stability_large);

    /* is_sorted */
    RUN_TEST(test_is_sorted_true);
    RUN_TEST(test_is_sorted_false);
    RUN_TEST(test_is_sorted_empty);
    RUN_TEST(test_is_sorted_single);
    RUN_TEST(test_is_sorted_with_dups);

    /* partial_sort */
    RUN_TEST(test_partial_sort_basic);
    RUN_TEST(test_partial_sort_k_equals_len);
    RUN_TEST(test_partial_sort_k_zero);
    RUN_TEST(test_partial_sort_k_one);
    RUN_TEST(test_partial_sort_desc);
    RUN_TEST(test_partial_sort_empty);

    /* nth_element */
    RUN_TEST(test_nth_element_median);
    RUN_TEST(test_nth_element_first);
    RUN_TEST(test_nth_element_last);
    RUN_TEST(test_nth_element_single);
    RUN_TEST(test_nth_element_all_equal);
    RUN_TEST(test_nth_element_partitions_correctly);

    return UNITY_END();
}
