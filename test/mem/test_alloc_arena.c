#include "unity.h"

#include "nx/mem/alloc_arena.h"

void setUp(void) {
}

void tearDown(void) {
}

static void test_nx_al_arena_alloc(void) {
    nx_al *al = nx_al_arena_new(1024); // 1KB
    TEST_ASSERT_TRUE(al != nx_null);

    const nx_usize len = 10;
    nx_i32 *arr = nx_al_alloc(al, len * sizeof(nx_i32));
    TEST_ASSERT_TRUE(arr != nx_null);

    for (nx_usize i = 0; i < len; ++i) {
        arr[i] = (nx_i32) (i * i);
    }

    for (nx_usize i = 0; i < len; ++i) {
        TEST_ASSERT_EQUAL_INT32(arr[i], (nx_i32)(i * i));
    }

    nx_al_dealloc(al, arr, len * sizeof(nx_i32));
    nx_al_free(al);
}

static void test_nx_al_arena_calloc(void) {
    nx_al *al = nx_al_arena_new(1024);
    TEST_ASSERT_TRUE(al != nx_null);

    const nx_usize len = 10;
    nx_i32 *arr = nx_al_calloc(al, len, sizeof(nx_i32));
    TEST_ASSERT_TRUE(arr != nx_null);

    for (nx_usize i = 0; i < len; ++i) {
        TEST_ASSERT_EQUAL_INT32(arr[i], 0);
    }

    for (nx_usize i = 0; i < len; ++i) {
        arr[i] = (nx_i32) (i * i);
    }

    for (nx_usize i = 0; i < len; ++i) {
        TEST_ASSERT_EQUAL_INT32(arr[i], (nx_i32)(i * i));
    }

    nx_al_dealloc(al, arr, len * sizeof(nx_i32));
    nx_al_free(al);
}

static void test_nx_al_arena_realloc(void) {
    nx_al *al = nx_al_arena_new(2048); // 2KB
    TEST_ASSERT_TRUE(al != nx_null);

    const nx_usize len = 10;
    nx_i32 *arr = nx_al_alloc(al, len * sizeof(nx_i32));
    TEST_ASSERT_TRUE(arr != nx_null);

    for (nx_usize i = 0; i < len; ++i) {
        arr[i] = (nx_i32) (i * i);
    }

    const nx_usize new_len = 20;
    nx_i32 *tmp = nx_al_realloc(al, arr, len * sizeof(nx_i32), new_len * sizeof(nx_i32));
    TEST_ASSERT_TRUE(tmp != nx_null);

    arr = tmp;

    for (nx_usize i = 0; i < len; ++i) {
        TEST_ASSERT_EQUAL_INT32(arr[i], (nx_i32)(i * i));
    }

    for (nx_usize i = len; i < new_len; ++i) {
        arr[i] = (nx_i32) (i * i * i);
    }

    for (nx_usize i = len; i < new_len; ++i) {
        TEST_ASSERT_EQUAL_INT32(arr[i], (nx_i32)(i * i * i));
    }

    nx_al_dealloc(al, arr, new_len * sizeof(nx_i32));
    nx_al_free(al);
}

static void test_nx_al_arena_reset(void) {
    nx_al *al = nx_al_arena_new(1024);
    TEST_ASSERT_TRUE(al != nx_null);

    nx_i32 *arr1 = nx_al_alloc(al, 100 * sizeof(nx_i32));
    TEST_ASSERT_TRUE(arr1 != nx_null);

    nx_al_arena_stats stats = nx_al_arena_get_stats(al);
    TEST_ASSERT_TRUE(stats.used > 0);
    TEST_ASSERT_TRUE(stats.used <= stats.cap);

    nx_al_arena_reset(al);

    stats = nx_al_arena_get_stats(al);
    TEST_ASSERT_EQUAL_UINT(0, stats.used);
    TEST_ASSERT_EQUAL_UINT(1024, stats.available);

    nx_i32 *arr2 = nx_al_alloc(al, 50 * sizeof(nx_i32));
    TEST_ASSERT_TRUE(arr2 != nx_null);

    TEST_ASSERT_EQUAL_PTR(arr1, arr2);

    nx_al_free(al);
}

static void test_nx_al_arena_stats(void) {
    const nx_usize capacity = 1024;
    nx_al *al = nx_al_arena_new(capacity);
    TEST_ASSERT_TRUE(al != nx_null);

    nx_al_arena_stats stats = nx_al_arena_get_stats(al);
    TEST_ASSERT_EQUAL_UINT(capacity, stats.cap);
    TEST_ASSERT_EQUAL_UINT(0, stats.used);
    TEST_ASSERT_EQUAL_UINT(capacity, stats.available);

    void *p1 = nx_al_alloc(al, 100);
    TEST_ASSERT_TRUE(p1 != nx_null);

    stats = nx_al_arena_get_stats(al);
    TEST_ASSERT_TRUE(stats.used > 0);
    TEST_ASSERT_TRUE(stats.used >= 100);
    TEST_ASSERT_EQUAL_UINT(capacity - stats.used, stats.available);

    void *p2 = nx_al_alloc(al, 200);
    TEST_ASSERT_TRUE(p2 != nx_null);

    nx_al_arena_stats stats2 = nx_al_arena_get_stats(al);
    TEST_ASSERT_TRUE(stats2.used > stats.used);
    TEST_ASSERT_EQUAL_UINT(capacity - stats2.used, stats2.available);

    nx_al_free(al);
}

static void test_nx_al_arena_oom(void) {
    nx_al *al = nx_al_arena_new(128);
    TEST_ASSERT_TRUE(al != nx_null);

    void *p1 = nx_al_alloc(al, 100);
    TEST_ASSERT_TRUE(p1 != nx_null);

    void *p2 = nx_al_alloc(al, 100);
    TEST_ASSERT_TRUE(p2 == nx_null); // OOM!

    void *p3 = nx_al_alloc(al, 10);
    TEST_ASSERT_TRUE(p3 != nx_null);

    nx_al_free(al);
}

static void test_nx_al_arena_alignment(void) {
    nx_al *al = nx_al_arena_new(1024);
    TEST_ASSERT_TRUE(al != nx_null);

    // Аллоцируем разные размеры
    void *p1 = nx_al_alloc(al, 1);
    void *p2 = nx_al_alloc(al, 1);
    void *p3 = nx_al_alloc(al, 1);

    TEST_ASSERT_TRUE(p1 != nx_null);
    TEST_ASSERT_TRUE(p2 != nx_null);
    TEST_ASSERT_TRUE(p3 != nx_null);

    // Проверяем что адреса выровнены по 8 байт
    TEST_ASSERT_EQUAL_UINT(0, ((nx_usize)p1) % 8);
    TEST_ASSERT_EQUAL_UINT(0, ((nx_usize)p2) % 8);
    TEST_ASSERT_EQUAL_UINT(0, ((nx_usize)p3) % 8);

    // Проверяем что между указателями 8 байт (alignment)
    TEST_ASSERT_EQUAL_UINT(8, (char*)p2 - (char*)p1);
    TEST_ASSERT_EQUAL_UINT(8, (char*)p3 - (char*)p2);

    nx_al_free(al);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_nx_al_arena_alloc);
    RUN_TEST(test_nx_al_arena_calloc);
    RUN_TEST(test_nx_al_arena_realloc);
    RUN_TEST(test_nx_al_arena_reset);
    RUN_TEST(test_nx_al_arena_stats);
    RUN_TEST(test_nx_al_arena_oom);
    RUN_TEST(test_nx_al_arena_alignment);

    return UNITY_END();
}
