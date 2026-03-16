#include "unity.h"

#include "nx/rand/rand.h"
#include "nx/core/cmp.h"
#include "nx/algo/sort.h"

void setUp(void) {
}

void tearDown(void) {
}

/* ========== lifetime ========== */

static void test_new_drop(void) {
    nx_Rng *r = nx_rng_new(42);
    TEST_ASSERT_NOT_NULL(r);
    nx_rng_drop(r);
}

static void test_drop_null(void) {
    nx_rng_drop(nx_null);
}

/* ========== determinism ========== */

static void test_deterministic(void) {
    nx_Rng *a = nx_rng_new(12345);
    nx_Rng *b = nx_rng_new(12345);

    for (int i = 0; i < 100; ++i) {
        TEST_ASSERT_EQUAL_UINT64(nx_rng_u64(a), nx_rng_u64(b));
    }

    nx_rng_drop(a);
    nx_rng_drop(b);
}

static void test_different_seeds(void) {
    nx_Rng *a = nx_rng_new(1);
    nx_Rng *b = nx_rng_new(2);

    // extremely unlikely to match
    nx_bool all_equal = true;
    for (int i = 0; i < 10; ++i) {
        if (nx_rng_u64(a) != nx_rng_u64(b)) {
            all_equal = false;
            break;
        }
    }
    TEST_ASSERT_FALSE(all_equal);

    nx_rng_drop(a);
    nx_rng_drop(b);
}

/* ========== u64 / u32 ========== */

static void test_u64(void) {
    nx_Rng *r = nx_rng_new(42);

    // just verify it doesn't crash and produces varying output
    nx_u64 prev = nx_rng_u64(r);
    nx_bool varied = false;
    for (int i = 0; i < 100; ++i) {
        const nx_u64 val = nx_rng_u64(r);
        if (val != prev) { varied = true; }
        prev = val;
    }
    TEST_ASSERT_TRUE(varied);

    nx_rng_drop(r);
}

static void test_u32(void) {
    nx_Rng *r = nx_rng_new(42);

    nx_u32 prev = nx_rng_u32(r);
    nx_bool varied = false;
    for (int i = 0; i < 100; ++i) {
        const nx_u32 val = nx_rng_u32(r);
        if (val != prev) { varied = true; }
        prev = val;
    }
    TEST_ASSERT_TRUE(varied);

    nx_rng_drop(r);
}

/* ========== bounded ========== */

static void test_u64_max(void) {
    nx_Rng *r = nx_rng_new(42);

    for (int i = 0; i < 1000; ++i) {
        const nx_u64 val = nx_rng_u64_max(r, 9);
        TEST_ASSERT_TRUE(val <= 9);
    }

    nx_rng_drop(r);
}

static void test_u32_max(void) {
    nx_Rng *r = nx_rng_new(42);

    for (int i = 0; i < 1000; ++i) {
        const nx_u32 val = nx_rng_u32_max(r, 5);
        TEST_ASSERT_TRUE(val <= 5);
    }

    nx_rng_drop(r);
}

static void test_u64_max_one(void) {
    nx_Rng *r = nx_rng_new(42);

    for (int i = 0; i < 100; ++i) {
        const nx_u64 val = nx_rng_u64_max(r, 0);
        TEST_ASSERT_EQUAL_UINT64(0, val);
    }

    nx_rng_drop(r);
}

static void test_i32_range(void) {
    nx_Rng *r = nx_rng_new(42);

    for (int i = 0; i < 1000; ++i) {
        const nx_i32 val = nx_rng_i32_range(r, -10, 10);
        TEST_ASSERT_TRUE(val >= -10 && val <= 10);
    }

    nx_rng_drop(r);
}

static void test_i64_range(void) {
    nx_Rng *r = nx_rng_new(42);

    for (int i = 0; i < 1000; ++i) {
        const nx_i64 val = nx_rng_i64_range(r, 100, 200);
        TEST_ASSERT_TRUE(val >= 100 && val <= 200);
    }

    nx_rng_drop(r);
}

static void test_i32_range_single(void) {
    nx_Rng *r = nx_rng_new(42);

    for (int i = 0; i < 100; ++i) {
        TEST_ASSERT_EQUAL_INT32(7, nx_rng_i32_range(r, 7, 7));
    }

    nx_rng_drop(r);
}

/* ========== float ========== */

static void test_f64(void) {
    nx_Rng *r = nx_rng_new(42);

    for (int i = 0; i < 10000; ++i) {
        const nx_f64 val = nx_rng_f64(r);
        TEST_ASSERT_TRUE(val >= 0.0 && val < 1.0);
    }

    nx_rng_drop(r);
}

static void test_f32(void) {
    nx_Rng *r = nx_rng_new(42);

    for (int i = 0; i < 10000; ++i) {
        const nx_f32 val = nx_rng_f32(r);
        TEST_ASSERT_TRUE(val >= 0.0f && val < 1.0f);
    }

    nx_rng_drop(r);
}

static void test_f64_range(void) {
    nx_Rng *r = nx_rng_new(42);

    for (int i = 0; i < 1000; ++i) {
        const nx_f64 val = nx_rng_f64_range(r, 5.0, 10.0);
        TEST_ASSERT_TRUE(val >= 5.0 && val < 10.0);
    }

    nx_rng_drop(r);
}

static void test_f32_range(void) {
    nx_Rng *r = nx_rng_new(42);

    for (int i = 0; i < 1000; ++i) {
        const nx_f32 val = nx_rng_f32_range(r, -1.0f, 1.0f);
        TEST_ASSERT_TRUE(val >= -1.0f && val < 1.0f);
    }

    nx_rng_drop(r);
}

/* ========== bool ========== */

static void test_bool(void) {
    nx_Rng *r = nx_rng_new(42);

    nx_usize trues = 0;
    constexpr nx_usize N = 10000;
    for (nx_usize i = 0; i < N; ++i) {
        if (nx_rng_bool(r)) { ++trues; }
    }

    // expect roughly 50% — allow [40%, 60%]
    TEST_ASSERT_TRUE(trues > N * 40 / 100);
    TEST_ASSERT_TRUE(trues < N * 60 / 100);

    nx_rng_drop(r);
}

/* ========== shuffle ========== */

static void test_shuffle(void) {
    nx_Rng *r = nx_rng_new(42);

    nx_i32 data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    constexpr nx_usize len = 10;
    const nx_Span s = NX_SPAN_FROM_ARR(data);

    nx_rng_shuffle(r, s);

    // verify it's a permutation: sort and check
    nx_i32 sorted[10];
    for (nx_usize i = 0; i < len; ++i) { sorted[i] = data[i]; }

    const nx_Span ss = NX_SPAN_FROM_ARR(sorted);
    nx_sort(ss, nx_cmp_i32);

    for (nx_usize i = 0; i < len; ++i) {
        TEST_ASSERT_EQUAL_INT32((nx_i32)(i + 1), sorted[i]);
    }

    // check it actually shuffled (not identity) — very unlikely for 10 elements
    nx_bool is_identity = true;
    for (nx_usize i = 0; i < len; ++i) {
        if (data[i] != (nx_i32) (i + 1)) {
            is_identity = false;
            break;
        }
    }
    TEST_ASSERT_FALSE(is_identity);

    nx_rng_drop(r);
}

static void test_shuffle_empty(void) {
    nx_Rng *r = nx_rng_new(42);

    const nx_Span s = nx_span_new(nx_null, 0, sizeof(nx_i32));
    nx_rng_shuffle(r, s); // must not crash

    nx_rng_drop(r);
}

static void test_shuffle_single(void) {
    nx_Rng *r = nx_rng_new(42);

    nx_i32 val = 42;
    const nx_Span s = NX_SPAN_FROM_PTR(&val, 1);
    nx_rng_shuffle(r, s);
    TEST_ASSERT_EQUAL_INT32(42, val);

    nx_rng_drop(r);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_new_drop);
    RUN_TEST(test_drop_null);

    RUN_TEST(test_deterministic);
    RUN_TEST(test_different_seeds);

    RUN_TEST(test_u64);
    RUN_TEST(test_u32);

    RUN_TEST(test_u64_max);
    RUN_TEST(test_u32_max);
    RUN_TEST(test_u64_max_one);
    RUN_TEST(test_i32_range);
    RUN_TEST(test_i64_range);
    RUN_TEST(test_i32_range_single);

    RUN_TEST(test_f64);
    RUN_TEST(test_f32);
    RUN_TEST(test_f64_range);
    RUN_TEST(test_f32_range);

    RUN_TEST(test_bool);

    RUN_TEST(test_shuffle);
    RUN_TEST(test_shuffle_empty);
    RUN_TEST(test_shuffle_single);

    return UNITY_END();
}
