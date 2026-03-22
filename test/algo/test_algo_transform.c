#include "unity.h"

#include <string.h>

#include "nx/algo/transform.h"
#include "nx/core/cmp.h"

void setUp(void) {
}

void tearDown(void) {
}

/* ========== helpers ========== */

static nx_bool is_even(const void *elem) {
    return (*(const nx_i32 *) elem) % 2 == 0;
}

static void double_val(void *dst, const void *src) {
    *(nx_i32 *) dst = *(const nx_i32 *) src * 2;
}

static nx_i32 gen_counter = 0;

static void gen_sequence(void *out) {
    *(nx_i32 *) out = gen_counter++;
}

static nx_i32 sum_acc = 0;

static void accumulate(const void *elem) {
    sum_acc += *(const nx_i32 *) elem;
}

static void negate_elem(void *elem) {
    *(nx_i32 *) elem = -(*(nx_i32 *) elem);
}

/* ========== fill ========== */

static void test_fill(void) {
    nx_i32 arr[5] = {0};
    nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_i32 val = 42;
    nx_fill(s, &val);
    for (int i = 0; i < 5; ++i) {
        TEST_ASSERT_EQUAL_INT32(42, arr[i]);
    }
}

static void test_fill_empty(void) {
    nx_Span s = nx_span_new(nx_null, 0, sizeof(nx_i32));
    nx_i32 val = 42;
    nx_fill(s, &val); // must not crash
}

/* ========== reverse ========== */

static void test_reverse(void) {
    nx_i32 arr[] = {1, 2, 3, 4, 5};
    nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_reverse(s);
    const nx_i32 expected[] = {5, 4, 3, 2, 1};
    TEST_ASSERT_EQUAL_MEMORY(expected, arr, sizeof(arr));
}

static void test_reverse_even_len(void) {
    nx_i32 arr[] = {1, 2, 3, 4};
    nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_reverse(s);
    const nx_i32 expected[] = {4, 3, 2, 1};
    TEST_ASSERT_EQUAL_MEMORY(expected, arr, sizeof(arr));
}

static void test_reverse_single(void) {
    nx_i32 val = 42;
    nx_Span s = NX_SPAN_FROM_PTR(&val, 1);
    nx_reverse(s);
    TEST_ASSERT_EQUAL_INT32(42, val);
}

static void test_reverse_empty(void) {
    nx_Span s = nx_span_new(nx_null, 0, sizeof(nx_i32));
    nx_reverse(s); // must not crash
}

/* ========== rotate ========== */

static void test_rotate(void) {
    nx_i32 arr[] = {1, 2, 3, 4, 5};
    nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_rotate(s, 2);
    const nx_i32 expected[] = {3, 4, 5, 1, 2};
    TEST_ASSERT_EQUAL_MEMORY(expected, arr, sizeof(arr));
}

static void test_rotate_zero(void) {
    nx_i32 arr[] = {1, 2, 3};
    nx_i32 original[] = {1, 2, 3};
    nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_rotate(s, 0);
    TEST_ASSERT_EQUAL_MEMORY(original, arr, sizeof(arr));
}

static void test_rotate_full(void) {
    nx_i32 arr[] = {1, 2, 3};
    nx_i32 original[] = {1, 2, 3};
    nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_rotate(s, 3);
    TEST_ASSERT_EQUAL_MEMORY(original, arr, sizeof(arr));
}

/* ========== transform ========== */

static void test_transform(void) {
    nx_i32 src[] = {1, 2, 3, 4};
    nx_i32 dst[4] = {0};
    nx_Span d = NX_SPAN_FROM_ARR(dst);
    nx_CSpan sr = NX_CSPAN_FROM_ARR(src);
    nx_transform(d, sr, double_val);
    const nx_i32 expected[] = {2, 4, 6, 8};
    TEST_ASSERT_EQUAL_MEMORY(expected, dst, sizeof(dst));
}

/* ========== copy ========== */

static void test_copy(void) {
    nx_i32 src[] = {10, 20, 30};
    nx_i32 dst[3] = {0};
    nx_copy(NX_SPAN_FROM_ARR(dst), NX_CSPAN_FROM_ARR(src));
    TEST_ASSERT_EQUAL_MEMORY(src, dst, sizeof(src));
}

/* ========== copy_if ========== */

static void test_copy_if(void) {
    nx_i32 src[] = {1, 2, 3, 4, 5, 6};
    nx_i32 dst[6] = {0};
    nx_usize n = nx_copy_if(NX_SPAN_FROM_ARR(dst), NX_CSPAN_FROM_ARR(src), is_even);
    TEST_ASSERT_EQUAL_UINT64(3, n);
    TEST_ASSERT_EQUAL_INT32(2, dst[0]);
    TEST_ASSERT_EQUAL_INT32(4, dst[1]);
    TEST_ASSERT_EQUAL_INT32(6, dst[2]);
}

static void test_copy_if_none(void) {
    nx_i32 src[] = {1, 3, 5};
    nx_i32 dst[3] = {0};
    nx_usize n = nx_copy_if(NX_SPAN_FROM_ARR(dst), NX_CSPAN_FROM_ARR(src), is_even);
    TEST_ASSERT_EQUAL_UINT64(0, n);
}

/* ========== replace ========== */

static void test_replace(void) {
    nx_i32 arr[] = {1, 2, 3, 2, 1};
    nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_i32 old_val = 2, new_val = 99;
    nx_usize n = nx_replace(s, &old_val, &new_val, nx_cmp_i32);
    TEST_ASSERT_EQUAL_UINT64(2, n);
    TEST_ASSERT_EQUAL_INT32(99, arr[1]);
    TEST_ASSERT_EQUAL_INT32(99, arr[3]);
}

static void test_replace_none(void) {
    nx_i32 arr[] = {1, 2, 3};
    nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_i32 old_val = 99, new_val = 0;
    nx_usize n = nx_replace(s, &old_val, &new_val, nx_cmp_i32);
    TEST_ASSERT_EQUAL_UINT64(0, n);
}

/* ========== replace_if ========== */

static void test_replace_if(void) {
    nx_i32 arr[] = {1, 2, 3, 4, 5};
    nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_i32 new_val = 0;
    nx_usize n = nx_replace_if(s, is_even, &new_val);
    TEST_ASSERT_EQUAL_UINT64(2, n);
    TEST_ASSERT_EQUAL_INT32(0, arr[1]);
    TEST_ASSERT_EQUAL_INT32(0, arr[3]);
}

/* ========== generate ========== */

static void test_generate(void) {
    gen_counter = 0;
    nx_i32 arr[5] = {0};
    nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_generate(s, gen_sequence);
    for (int i = 0; i < 5; ++i) {
        TEST_ASSERT_EQUAL_INT32(i, arr[i]);
    }
}

/* ========== for_each / for_each_c ========== */

static void test_for_each(void) {
    nx_i32 arr[] = {1, 2, 3};
    nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_for_each(s, negate_elem);
    TEST_ASSERT_EQUAL_INT32(-1, arr[0]);
    TEST_ASSERT_EQUAL_INT32(-2, arr[1]);
    TEST_ASSERT_EQUAL_INT32(-3, arr[2]);
}

static void test_for_each_c(void) {
    nx_i32 arr[] = {10, 20, 30};
    nx_CSpan s = NX_CSPAN_FROM_ARR(arr);
    sum_acc = 0;
    nx_for_each_c(s, accumulate);
    TEST_ASSERT_EQUAL_INT32(60, sum_acc);
}

/* ========== remove ========== */

static void test_remove(void) {
    nx_i32 arr[] = {1, 2, 3, 2, 4, 2, 5};
    nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_i32 key = 2;
    nx_usize new_len = nx_remove(s, &key, nx_cmp_i32);
    TEST_ASSERT_EQUAL_UINT64(4, new_len);
    TEST_ASSERT_EQUAL_INT32(1, arr[0]);
    TEST_ASSERT_EQUAL_INT32(3, arr[1]);
    TEST_ASSERT_EQUAL_INT32(4, arr[2]);
    TEST_ASSERT_EQUAL_INT32(5, arr[3]);
}

static void test_remove_none(void) {
    nx_i32 arr[] = {1, 2, 3};
    nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_i32 key = 99;
    nx_usize new_len = nx_remove(s, &key, nx_cmp_i32);
    TEST_ASSERT_EQUAL_UINT64(3, new_len);
}

/* ========== remove_if ========== */

static void test_remove_if(void) {
    nx_i32 arr[] = {1, 2, 3, 4, 5, 6};
    nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_usize new_len = nx_remove_if(s, is_even);
    TEST_ASSERT_EQUAL_UINT64(3, new_len);
    TEST_ASSERT_EQUAL_INT32(1, arr[0]);
    TEST_ASSERT_EQUAL_INT32(3, arr[1]);
    TEST_ASSERT_EQUAL_INT32(5, arr[2]);
}

/* ========== unique (sorted input) ========== */

static void test_unique(void) {
    nx_i32 arr[] = {1, 1, 2, 2, 2, 3, 4, 4, 5};
    nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_usize new_len = nx_unique(s, nx_cmp_i32);
    TEST_ASSERT_EQUAL_UINT64(5, new_len);
    TEST_ASSERT_EQUAL_INT32(1, arr[0]);
    TEST_ASSERT_EQUAL_INT32(2, arr[1]);
    TEST_ASSERT_EQUAL_INT32(3, arr[2]);
    TEST_ASSERT_EQUAL_INT32(4, arr[3]);
    TEST_ASSERT_EQUAL_INT32(5, arr[4]);
}

static void test_unique_no_dups(void) {
    nx_i32 arr[] = {1, 2, 3};
    nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_usize new_len = nx_unique(s, nx_cmp_i32);
    TEST_ASSERT_EQUAL_UINT64(3, new_len);
}

static void test_unique_all_same(void) {
    nx_i32 arr[] = {7, 7, 7, 7};
    nx_Span s = NX_SPAN_FROM_ARR(arr);
    nx_usize new_len = nx_unique(s, nx_cmp_i32);
    TEST_ASSERT_EQUAL_UINT64(1, new_len);
    TEST_ASSERT_EQUAL_INT32(7, arr[0]);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_fill);
    RUN_TEST(test_fill_empty);

    RUN_TEST(test_reverse);
    RUN_TEST(test_reverse_even_len);
    RUN_TEST(test_reverse_single);
    RUN_TEST(test_reverse_empty);

    RUN_TEST(test_rotate);
    RUN_TEST(test_rotate_zero);
    RUN_TEST(test_rotate_full);

    RUN_TEST(test_transform);

    RUN_TEST(test_copy);
    RUN_TEST(test_copy_if);
    RUN_TEST(test_copy_if_none);

    RUN_TEST(test_replace);
    RUN_TEST(test_replace_none);
    RUN_TEST(test_replace_if);

    RUN_TEST(test_generate);

    RUN_TEST(test_for_each);
    RUN_TEST(test_for_each_c);

    RUN_TEST(test_remove);
    RUN_TEST(test_remove_none);
    RUN_TEST(test_remove_if);

    RUN_TEST(test_unique);
    RUN_TEST(test_unique_no_dups);
    RUN_TEST(test_unique_all_same);

    return UNITY_END();
}
