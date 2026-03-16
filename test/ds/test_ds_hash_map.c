#include "unity.h"

#include "nx/ds/hash_map.h"
#include "nx/core/hash.h"
#include "nx/core/cmp.h"
#include "nx/mem/alloc_arena.h"

#include <string.h>

void setUp(void) {
}

void tearDown(void) {
}

/* ========== basic ========== */

static void test_new_and_drop(void) {
    nx_HashMapRes res = NX_HASH_MAP_NEW(nx_i32, nx_i32, nx_hash_fn_i32, nx_eq_fn_i32);
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_HashMap *m = NX_RES_UNWRAP(res);
    TEST_ASSERT_EQUAL_UINT64(0, nx_hash_map_len(m));
    TEST_ASSERT_TRUE(nx_hash_map_empty(m));

    nx_hash_map_drop(m);
}

static void test_put_and_get(void) {
    nx_HashMapRes res = NX_HASH_MAP_NEW(nx_i32, nx_i32, nx_hash_fn_i32, nx_eq_fn_i32);
    nx_HashMap *m = NX_RES_UNWRAP(res);

    nx_i32 k = 42, v = 100;
    TEST_ASSERT_EQUAL(NX_STATUS_OK, nx_hash_map_put(m, &k, &v));
    TEST_ASSERT_EQUAL_UINT64(1, nx_hash_map_len(m));

    nx_i32 *got = NX_HASH_MAP_GET_AS(nx_i32, m, &k);
    TEST_ASSERT_NOT_NULL(got);
    TEST_ASSERT_EQUAL_INT32(100, *got);

    nx_hash_map_drop(m);
}

static void test_get_missing(void) {
    nx_HashMapRes res = NX_HASH_MAP_NEW(nx_i32, nx_i32, nx_hash_fn_i32, nx_eq_fn_i32);
    nx_HashMap *m = NX_RES_UNWRAP(res);

    nx_i32 k = 1, v = 2;
    nx_hash_map_put(m, &k, &v);

    nx_i32 missing = 999;
    TEST_ASSERT_NULL(nx_hash_map_get(m, &missing));
    TEST_ASSERT_FALSE(nx_hash_map_contains(m, &missing));

    nx_hash_map_drop(m);
}

static void test_overwrite(void) {
    nx_HashMapRes res = NX_HASH_MAP_NEW(nx_i32, nx_i32, nx_hash_fn_i32, nx_eq_fn_i32);
    nx_HashMap *m = NX_RES_UNWRAP(res);

    nx_i32 k = 5;
    nx_i32 v1 = 10, v2 = 20;

    nx_hash_map_put(m, &k, &v1);
    TEST_ASSERT_EQUAL_INT32(10, *NX_HASH_MAP_GET_AS(nx_i32, m, &k));

    nx_hash_map_put(m, &k, &v2);
    TEST_ASSERT_EQUAL_INT32(20, *NX_HASH_MAP_GET_AS(nx_i32, m, &k));
    TEST_ASSERT_EQUAL_UINT64(1, nx_hash_map_len(m)); // still 1

    nx_hash_map_drop(m);
}

static void test_contains(void) {
    nx_HashMapRes res = NX_HASH_MAP_NEW(nx_i32, nx_i32, nx_hash_fn_i32, nx_eq_fn_i32);
    nx_HashMap *m = NX_RES_UNWRAP(res);

    nx_i32 k = 7, v = 77;
    TEST_ASSERT_FALSE(nx_hash_map_contains(m, &k));

    nx_hash_map_put(m, &k, &v);
    TEST_ASSERT_TRUE(nx_hash_map_contains(m, &k));

    nx_hash_map_drop(m);
}

/* ========== remove ========== */

static void test_remove(void) {
    nx_HashMapRes res = NX_HASH_MAP_NEW(nx_i32, nx_i32, nx_hash_fn_i32, nx_eq_fn_i32);
    nx_HashMap *m = NX_RES_UNWRAP(res);

    nx_i32 k = 1, v = 10;
    nx_hash_map_put(m, &k, &v);
    TEST_ASSERT_EQUAL_UINT64(1, nx_hash_map_len(m));

    TEST_ASSERT_TRUE(nx_hash_map_remove(m, &k));
    TEST_ASSERT_EQUAL_UINT64(0, nx_hash_map_len(m));
    TEST_ASSERT_NULL(nx_hash_map_get(m, &k));

    nx_hash_map_drop(m);
}

static void test_remove_missing(void) {
    nx_HashMapRes res = NX_HASH_MAP_NEW(nx_i32, nx_i32, nx_hash_fn_i32, nx_eq_fn_i32);
    nx_HashMap *m = NX_RES_UNWRAP(res);

    nx_i32 k = 42;
    TEST_ASSERT_FALSE(nx_hash_map_remove(m, &k));

    nx_hash_map_drop(m);
}

static void test_remove_and_reinsert(void) {
    nx_HashMapRes res = NX_HASH_MAP_NEW(nx_i32, nx_i32, nx_hash_fn_i32, nx_eq_fn_i32);
    nx_HashMap *m = NX_RES_UNWRAP(res);

    for (nx_i32 i = 0; i < 10; ++i) {
        nx_i32 v = i * 10;
        nx_hash_map_put(m, &i, &v);
    }
    TEST_ASSERT_EQUAL_UINT64(10, nx_hash_map_len(m));

    // remove evens
    for (nx_i32 i = 0; i < 10; i += 2) {
        TEST_ASSERT_TRUE(nx_hash_map_remove(m, &i));
    }
    TEST_ASSERT_EQUAL_UINT64(5, nx_hash_map_len(m));

    // odds still there
    for (nx_i32 i = 1; i < 10; i += 2) {
        nx_i32 *v = NX_HASH_MAP_GET_AS(nx_i32, m, &i);
        TEST_ASSERT_NOT_NULL(v);
        TEST_ASSERT_EQUAL_INT32(i * 10, *v);
    }

    // re-insert evens with new values
    for (nx_i32 i = 0; i < 10; i += 2) {
        nx_i32 v = i * 100;
        nx_hash_map_put(m, &i, &v);
    }
    TEST_ASSERT_EQUAL_UINT64(10, nx_hash_map_len(m));

    for (nx_i32 i = 0; i < 10; i += 2) {
        TEST_ASSERT_EQUAL_INT32(i * 100, *NX_HASH_MAP_GET_AS(nx_i32, m, &i));
    }

    nx_hash_map_drop(m);
}

/* ========== growth / rehash ========== */

static void test_many_entries(void) {
    nx_HashMapRes res = NX_HASH_MAP_NEW(nx_i32, nx_i64, nx_hash_fn_i32, nx_eq_fn_i32);
    nx_HashMap *m = NX_RES_UNWRAP(res);

    constexpr nx_i32 N = 1000;

    for (nx_i32 i = 0; i < N; ++i) {
        nx_i64 v = (nx_i64) i * (nx_i64) i;
        TEST_ASSERT_EQUAL(NX_STATUS_OK, nx_hash_map_put(m, &i, &v));
    }
    TEST_ASSERT_EQUAL_UINT64(N, nx_hash_map_len(m));

    for (nx_i32 i = 0; i < N; ++i) {
        const nx_i64 *v = NX_HASH_MAP_GET_AS_C(nx_i64, m, &i);
        TEST_ASSERT_NOT_NULL(v);
        TEST_ASSERT_EQUAL_INT64((nx_i64) i * (nx_i64) i, *v);
    }

    // remove half
    for (nx_i32 i = 0; i < N; i += 2) {
        TEST_ASSERT_TRUE(nx_hash_map_remove(m, &i));
    }
    TEST_ASSERT_EQUAL_UINT64(N / 2, nx_hash_map_len(m));

    // remaining half intact
    for (nx_i32 i = 1; i < N; i += 2) {
        TEST_ASSERT_EQUAL_INT64((nx_i64) i * (nx_i64) i, *NX_HASH_MAP_GET_AS_C(nx_i64, m, &i));
    }

    nx_hash_map_drop(m);
}

/* ========== clear ========== */

static void test_clear(void) {
    nx_HashMapRes res = NX_HASH_MAP_NEW(nx_i32, nx_i32, nx_hash_fn_i32, nx_eq_fn_i32);
    nx_HashMap *m = NX_RES_UNWRAP(res);

    for (nx_i32 i = 0; i < 20; ++i) {
        nx_hash_map_put(m, &i, &i);
    }
    TEST_ASSERT_EQUAL_UINT64(20, nx_hash_map_len(m));

    nx_hash_map_clear(m);
    TEST_ASSERT_EQUAL_UINT64(0, nx_hash_map_len(m));
    TEST_ASSERT_TRUE(nx_hash_map_empty(m));

    // can reuse
    nx_i32 k = 1, v = 99;
    nx_hash_map_put(m, &k, &v);
    TEST_ASSERT_EQUAL_INT32(99, *NX_HASH_MAP_GET_AS(nx_i32, m, &k));

    nx_hash_map_drop(m);
}

/* ========== reserve ========== */

static void test_reserve(void) {
    nx_HashMapRes res = NX_HASH_MAP_NEW(nx_i32, nx_i32, nx_hash_fn_i32, nx_eq_fn_i32);
    nx_HashMap *m = NX_RES_UNWRAP(res);

    TEST_ASSERT_EQUAL(NX_STATUS_OK, nx_hash_map_reserve(m, 100));
    TEST_ASSERT_TRUE(nx_hash_map_cap(m) >= 100);
    TEST_ASSERT_EQUAL_UINT64(0, nx_hash_map_len(m));

    // still works normally
    for (nx_i32 i = 0; i < 100; ++i) {
        nx_hash_map_put(m, &i, &i);
    }
    TEST_ASSERT_EQUAL_UINT64(100, nx_hash_map_len(m));

    nx_hash_map_drop(m);
}

/* ========== string keys ========== */

static void test_string_keys(void) {
    nx_HashMapRes res = NX_HASH_MAP_NEW(const nx_char *, nx_i32, nx_hash_fn_cstr, nx_eq_fn_cstr);
    nx_HashMap *m = NX_RES_UNWRAP(res);

    const nx_char *k1 = "hello";
    const nx_char *k2 = "world";
    const nx_char *k3 = "foo";
    nx_i32 v1 = 1, v2 = 2, v3 = 3;

    nx_hash_map_put(m, &k1, &v1);
    nx_hash_map_put(m, &k2, &v2);
    nx_hash_map_put(m, &k3, &v3);

    TEST_ASSERT_EQUAL_INT32(1, *NX_HASH_MAP_GET_AS(nx_i32, m, &k1));
    TEST_ASSERT_EQUAL_INT32(2, *NX_HASH_MAP_GET_AS(nx_i32, m, &k2));
    TEST_ASSERT_EQUAL_INT32(3, *NX_HASH_MAP_GET_AS(nx_i32, m, &k3));

    // lookup with different pointer, same string
    const nx_char *k1_copy = "hello";
    TEST_ASSERT_EQUAL_INT32(1, *NX_HASH_MAP_GET_AS(nx_i32, m, &k1_copy));

    nx_hash_map_drop(m);
}

/* ========== iteration ========== */

static void sum_values(const void *key, const void *val, void *ctx) {
    NX_UNUSED(key);
    nx_i64 *sum = ctx;
    *sum += *(const nx_i32 *) val;
}

static void test_for_each(void) {
    nx_HashMapRes res = NX_HASH_MAP_NEW(nx_i32, nx_i32, nx_hash_fn_i32, nx_eq_fn_i32);
    nx_HashMap *m = NX_RES_UNWRAP(res);

    nx_i64 expected = 0;
    for (nx_i32 i = 0; i < 10; ++i) {
        nx_hash_map_put(m, &i, &i);
        expected += i;
    }

    nx_i64 sum = 0;
    nx_hash_map_for_each_c(m, sum_values, &sum);
    TEST_ASSERT_EQUAL_INT64(expected, sum);

    nx_hash_map_drop(m);
}

/* ========== macro helpers ========== */

static void test_put_macro(void) {
    nx_HashMapRes res = NX_HASH_MAP_NEW(nx_i32, nx_i32, nx_hash_fn_i32, nx_eq_fn_i32);
    nx_HashMap *m = NX_RES_UNWRAP(res);

    NX_HASH_MAP_PUT(nx_i32, nx_i32, m, 42, 100);
    NX_HASH_MAP_PUT(nx_i32, nx_i32, m, 43, 200);

    nx_i32 k = 42;
    TEST_ASSERT_EQUAL_INT32(100, *NX_HASH_MAP_GET_AS(nx_i32, m, &k));
    k = 43;
    TEST_ASSERT_EQUAL_INT32(200, *NX_HASH_MAP_GET_AS(nx_i32, m, &k));

    nx_hash_map_drop(m);
}

/* ========== custom allocator ========== */

static void test_with_arena(void) {
    nx_Al *arena = nx_al_arena_new(64 * 1024);
    TEST_ASSERT_NOT_NULL(arena);

    nx_HashMapRes res = nx_hash_map_new_p((nx_HashMapParams){
        .tsz_key = sizeof(nx_i32),
        .tsz_val = sizeof(nx_i32),
        .hash = nx_hash_fn_i32,
        .eq = nx_eq_fn_i32,
        .al = arena,
    });
    nx_HashMap *m = NX_RES_UNWRAP(res);

    for (nx_i32 i = 0; i < 100; ++i) {
        nx_i32 v = i * 2;
        nx_hash_map_put(m, &i, &v);
    }

    for (nx_i32 i = 0; i < 100; ++i) {
        TEST_ASSERT_EQUAL_INT32(i * 2, *NX_HASH_MAP_GET_AS(nx_i32, m, &i));
    }

    nx_hash_map_drop(m);
    nx_al_arena_drop(arena);
}

/* ========== empty map edge cases ========== */

static void test_get_from_empty(void) {
    nx_HashMapRes res = NX_HASH_MAP_NEW(nx_i32, nx_i32, nx_hash_fn_i32, nx_eq_fn_i32);
    nx_HashMap *m = NX_RES_UNWRAP(res);

    nx_i32 k = 0;
    TEST_ASSERT_NULL(nx_hash_map_get(m, &k));
    TEST_ASSERT_FALSE(nx_hash_map_contains(m, &k));
    TEST_ASSERT_FALSE(nx_hash_map_remove(m, &k));

    nx_hash_map_drop(m);
}

static void test_drop_null(void) {
    nx_hash_map_drop(nx_null); // should not crash
}

/* ========== large struct value ========== */

typedef struct {
    nx_i64 a, b, c, d;
} BigVal;

static void test_large_value(void) {
    nx_HashMapRes res = NX_HASH_MAP_NEW(nx_i32, BigVal, nx_hash_fn_i32, nx_eq_fn_i32);
    nx_HashMap *m = NX_RES_UNWRAP(res);

    for (nx_i32 i = 0; i < 50; ++i) {
        BigVal v = {.a = i, .b = i * 2, .c = i * 3, .d = i * 4};
        nx_hash_map_put(m, &i, &v);
    }

    for (nx_i32 i = 0; i < 50; ++i) {
        const BigVal *v = NX_HASH_MAP_GET_AS_C(BigVal, m, &i);
        TEST_ASSERT_NOT_NULL(v);
        TEST_ASSERT_EQUAL_INT64(i, v->a);
        TEST_ASSERT_EQUAL_INT64(i * 4, v->d);
    }

    nx_hash_map_drop(m);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_new_and_drop);
    RUN_TEST(test_put_and_get);
    RUN_TEST(test_get_missing);
    RUN_TEST(test_overwrite);
    RUN_TEST(test_contains);

    RUN_TEST(test_remove);
    RUN_TEST(test_remove_missing);
    RUN_TEST(test_remove_and_reinsert);

    RUN_TEST(test_many_entries);
    RUN_TEST(test_clear);
    RUN_TEST(test_reserve);

    RUN_TEST(test_string_keys);
    RUN_TEST(test_for_each);
    RUN_TEST(test_put_macro);
    RUN_TEST(test_with_arena);

    RUN_TEST(test_get_from_empty);
    RUN_TEST(test_drop_null);
    RUN_TEST(test_large_value);

    return UNITY_END();
}
