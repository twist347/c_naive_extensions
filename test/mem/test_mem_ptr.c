#include "unity.h"

#include <string.h>

#include "nx/mem/ptr.h"

void setUp(void) {
}

void tearDown(void) {
}

/* ========== byte_offset ========== */

static void test_byte_offset(void) {
    nx_i32 arr[] = {10, 20, 30, 40};
    nx_byte *p = nx_byte_offset(arr, sizeof(nx_i32), 2);
    TEST_ASSERT_EQUAL_INT32(30, *(nx_i32 *) p);
}

static void test_byte_offset_c(void) {
    const nx_i32 arr[] = {10, 20, 30, 40};
    const nx_byte *p = nx_byte_offset_c(arr, sizeof(nx_i32), 3);
    TEST_ASSERT_EQUAL_INT32(40, *(const nx_i32 *) p);
}

static void test_byte_offset_zero(void) {
    nx_i32 arr[] = {99};
    nx_byte *p = nx_byte_offset(arr, sizeof(nx_i32), 0);
    TEST_ASSERT_EQUAL_PTR(arr, p);
}

/* ========== byte_diff ========== */

static void test_byte_diff(void) {
    nx_i32 arr[4] = {0};
    nx_isize d = nx_byte_diff(&arr[3], &arr[0]);
    TEST_ASSERT_EQUAL_INT64(3 * (nx_isize) sizeof(nx_i32), d);
}

static void test_byte_diff_same(void) {
    nx_i32 x = 0;
    TEST_ASSERT_EQUAL_INT64(0, nx_byte_diff(&x, &x));
}

/* ========== ptr_distance ========== */

static void test_ptr_distance(void) {
    nx_i64 arr[8] = {0};
    nx_usize d = nx_ptr_distance(&arr[5], &arr[2], sizeof(nx_i64));
    TEST_ASSERT_EQUAL_UINT64(3, d);
}

static void test_ptr_distance_zero(void) {
    nx_i32 x = 0;
    TEST_ASSERT_EQUAL_UINT64(0, nx_ptr_distance(&x, &x, sizeof(nx_i32)));
}

static void test_ptr_distance_one_byte_stride(void) {
    nx_byte buf[16] = {0};
    TEST_ASSERT_EQUAL_UINT64(7, nx_ptr_distance(&buf[10], &buf[3], 1));
}

/* ========== align_up / align_down ========== */

static void test_align_up(void) {
    TEST_ASSERT_EQUAL_UINT64(16, nx_align_up(1, 16));
    TEST_ASSERT_EQUAL_UINT64(16, nx_align_up(16, 16));
    TEST_ASSERT_EQUAL_UINT64(32, nx_align_up(17, 16));
    TEST_ASSERT_EQUAL_UINT64(0, nx_align_up(0, 8));
}

static void test_align_down(void) {
    TEST_ASSERT_EQUAL_UINT64(0, nx_align_down(1, 16));
    TEST_ASSERT_EQUAL_UINT64(16, nx_align_down(16, 16));
    TEST_ASSERT_EQUAL_UINT64(16, nx_align_down(31, 16));
    TEST_ASSERT_EQUAL_UINT64(0, nx_align_down(0, 8));
}

/* ========== is_aligned ========== */

static void test_is_aligned(void) {
    alignas(64) nx_byte buf[64] = {0};
    TEST_ASSERT_TRUE(nx_is_aligned(buf, 1));
    TEST_ASSERT_TRUE(nx_is_aligned(buf, 2));
    TEST_ASSERT_TRUE(nx_is_aligned(buf, 4));
    TEST_ASSERT_TRUE(nx_is_aligned(buf, 8));
    TEST_ASSERT_TRUE(nx_is_aligned(buf, 16));
    TEST_ASSERT_TRUE(nx_is_aligned(buf, 32));
    TEST_ASSERT_TRUE(nx_is_aligned(buf, 64));

    // buf + 1 is NOT aligned to anything > 1
    TEST_ASSERT_FALSE(nx_is_aligned(buf + 1, 2));
    TEST_ASSERT_FALSE(nx_is_aligned(buf + 1, 4));
}

/* ========== align_ptr_up ========== */

static void test_align_ptr_up_already_aligned(void) {
    alignas(64) nx_byte buf[64] = {0};
    void *p = nx_align_ptr_up(buf, 16);
    TEST_ASSERT_EQUAL_PTR(buf, p);
}

static void test_align_ptr_up_needs_bump(void) {
    alignas(64) nx_byte buf[128] = {0};
    void *p = nx_align_ptr_up(buf + 1, 16);
    TEST_ASSERT_TRUE((nx_uptr) p >= (nx_uptr) (buf + 1));
    TEST_ASSERT_TRUE(nx_is_aligned(p, 16));
    // should be buf + 16
    TEST_ASSERT_EQUAL_PTR(buf + 16, p);
}

/* ========== ptr_in_range ========== */

static void test_ptr_in_range_inside(void) {
    nx_i32 arr[10] = {0};
    const void *lo = arr;
    const void *hi = arr + 10;
    TEST_ASSERT_TRUE(nx_ptr_in_range(&arr[0], lo, hi));
    TEST_ASSERT_TRUE(nx_ptr_in_range(&arr[5], lo, hi));
    TEST_ASSERT_TRUE(nx_ptr_in_range(&arr[9], lo, hi));
}

static void test_ptr_in_range_outside(void) {
    nx_i32 arr[10] = {0};
    const void *lo = arr;
    const void *hi = arr + 10;
    // one past end is outside
    TEST_ASSERT_FALSE(nx_ptr_in_range(&arr[10], lo, hi));
}

static void test_ptr_in_range_byte_stride(void) {
    nx_byte buf[32] = {0};
    const void *lo = buf;
    const void *hi = buf + 32;
    TEST_ASSERT_TRUE(nx_ptr_in_range(&buf[31], lo, hi));
    TEST_ASSERT_FALSE(nx_ptr_in_range(buf + 32, lo, hi));
}

/* ========== ptr_exchange ========== */

static void test_ptr_exchange(void) {
    nx_i32 a = 1, b = 2;
    void *ptr = &a;
    void *old = nx_ptr_exchange(&ptr, &b);
    TEST_ASSERT_EQUAL_PTR(&a, old);
    TEST_ASSERT_EQUAL_PTR(&b, ptr);
}

/* ========== memswap ========== */

static void test_memswap_basic(void) {
    nx_i32 a = 42, b = 99;
    nx_memswap(&a, &b, sizeof(nx_i32));
    TEST_ASSERT_EQUAL_INT32(99, a);
    TEST_ASSERT_EQUAL_INT32(42, b);
}

static void test_memswap_array(void) {
    nx_i64 a[] = {1, 2, 3, 4};
    nx_i64 b[] = {10, 20, 30, 40};

    nx_memswap(a, b, sizeof(a));

    TEST_ASSERT_EQUAL_INT64(10, a[0]);
    TEST_ASSERT_EQUAL_INT64(20, a[1]);
    TEST_ASSERT_EQUAL_INT64(30, a[2]);
    TEST_ASSERT_EQUAL_INT64(40, a[3]);

    TEST_ASSERT_EQUAL_INT64(1, b[0]);
    TEST_ASSERT_EQUAL_INT64(2, b[1]);
    TEST_ASSERT_EQUAL_INT64(3, b[2]);
    TEST_ASSERT_EQUAL_INT64(4, b[3]);
}

static void test_memswap_odd_size(void) {
    // 7 bytes — not word-aligned
    nx_byte a[7] = {1, 2, 3, 4, 5, 6, 7};
    nx_byte b[7] = {10, 20, 30, 40, 50, 60, 70};

    nx_memswap(a, b, 7);

    const nx_byte expected_a[] = {10, 20, 30, 40, 50, 60, 70};
    const nx_byte expected_b[] = {1, 2, 3, 4, 5, 6, 7};
    TEST_ASSERT_EQUAL_MEMORY(expected_a, a, 7);
    TEST_ASSERT_EQUAL_MEMORY(expected_b, b, 7);
}

static void test_memswap_same_ptr(void) {
    nx_i32 a = 42;
    nx_memswap(&a, &a, sizeof(nx_i32)); // no-op
    TEST_ASSERT_EQUAL_INT32(42, a);
}

static void test_memswap_zero_size(void) {
    nx_i32 a = 42, b = 99;
    nx_memswap(&a, &b, 0); // no-op
    TEST_ASSERT_EQUAL_INT32(42, a);
    TEST_ASSERT_EQUAL_INT32(99, b);
}

static void test_memswap_single_byte(void) {
    nx_byte a = 0xAA, b = 0x55;
    nx_memswap(&a, &b, 1);
    TEST_ASSERT_EQUAL_HEX8(0x55, a);
    TEST_ASSERT_EQUAL_HEX8(0xAA, b);
}

static void test_memswap_large_struct(void) {
    typedef struct {
        nx_i64 vals[16];
    } Big;

    Big a, b;
    for (int i = 0; i < 16; ++i) {
        a.vals[i] = i;
        b.vals[i] = 100 + i;
    }

    nx_memswap(&a, &b, sizeof(Big));

    for (int i = 0; i < 16; ++i) {
        TEST_ASSERT_EQUAL_INT64(100 + i, a.vals[i]);
        TEST_ASSERT_EQUAL_INT64(i, b.vals[i]);
    }
}

/* ========== memzero ========== */

static void test_memzero(void) {
    nx_i32 arr[] = {1, 2, 3, 4};
    nx_memzero(arr, sizeof(arr));
    for (int i = 0; i < 4; ++i) {
        TEST_ASSERT_EQUAL_INT32(0, arr[i]);
    }
}

static void test_memzero_partial(void) {
    nx_byte buf[8] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    nx_memzero(buf, 4);
    for (int i = 0; i < 4; ++i) {
        TEST_ASSERT_EQUAL_HEX8(0x00, buf[i]);
    }
    for (int i = 4; i < 8; ++i) {
        TEST_ASSERT_EQUAL_HEX8(0xFF, buf[i]);
    }
}

/* ========== memeq ========== */

static void test_memeq_equal(void) {
    nx_i32 a[] = {1, 2, 3};
    nx_i32 b[] = {1, 2, 3};
    TEST_ASSERT_TRUE(nx_memeq(a, b, sizeof(a)));
}

static void test_memeq_not_equal(void) {
    nx_i32 a[] = {1, 2, 3};
    nx_i32 b[] = {1, 2, 4};
    TEST_ASSERT_FALSE(nx_memeq(a, b, sizeof(a)));
}

static void test_memeq_zero_len(void) {
    nx_i32 a = 1, b = 999;
    TEST_ASSERT_TRUE(nx_memeq(&a, &b, 0));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_byte_offset);
    RUN_TEST(test_byte_offset_c);
    RUN_TEST(test_byte_offset_zero);

    RUN_TEST(test_byte_diff);
    RUN_TEST(test_byte_diff_same);

    RUN_TEST(test_ptr_distance);
    RUN_TEST(test_ptr_distance_zero);
    RUN_TEST(test_ptr_distance_one_byte_stride);

    RUN_TEST(test_align_up);
    RUN_TEST(test_align_down);
    RUN_TEST(test_is_aligned);
    RUN_TEST(test_align_ptr_up_already_aligned);
    RUN_TEST(test_align_ptr_up_needs_bump);

    RUN_TEST(test_ptr_in_range_inside);
    RUN_TEST(test_ptr_in_range_outside);
    RUN_TEST(test_ptr_in_range_byte_stride);

    RUN_TEST(test_ptr_exchange);

    RUN_TEST(test_memswap_basic);
    RUN_TEST(test_memswap_array);
    RUN_TEST(test_memswap_odd_size);
    RUN_TEST(test_memswap_same_ptr);
    RUN_TEST(test_memswap_zero_size);
    RUN_TEST(test_memswap_single_byte);
    RUN_TEST(test_memswap_large_struct);

    RUN_TEST(test_memzero);
    RUN_TEST(test_memzero_partial);

    RUN_TEST(test_memeq_equal);
    RUN_TEST(test_memeq_not_equal);
    RUN_TEST(test_memeq_zero_len);

    return UNITY_END();
}
