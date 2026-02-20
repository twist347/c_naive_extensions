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
    nx_i32 *arr = NX_AL_ALLOC(nx_i32, al, len);
    TEST_ASSERT_TRUE(arr != nx_null);

    for (nx_usize i = 0; i < len; ++i) {
        arr[i] = (nx_i32) (i * i);
    }

    for (nx_usize i = 0; i < len; ++i) {
        TEST_ASSERT_EQUAL_INT32(arr[i], (nx_i32) (i * i));
    }

    NX_AL_DEALLOC(nx_i32, al, arr, len);
    nx_al_arena_drop(al);
}

static void test_nx_al_arena_calloc(void) {
    nx_al *al = nx_al_arena_new(1024);
    TEST_ASSERT_TRUE(al != nx_null);

    const nx_usize len = 10;
    nx_i32 *arr = NX_AL_CALLOC(nx_i32, al, len);
    TEST_ASSERT_TRUE(arr != nx_null);

    for (nx_usize i = 0; i < len; ++i) {
        TEST_ASSERT_EQUAL_INT32(arr[i], 0);
    }

    for (nx_usize i = 0; i < len; ++i) {
        arr[i] = (nx_i32) (i * i);
    }

    for (nx_usize i = 0; i < len; ++i) {
        TEST_ASSERT_EQUAL_INT32(arr[i], (nx_i32) (i * i));
    }

    NX_AL_DEALLOC(nx_i32, al, arr, len);
    nx_al_arena_drop(al);
}

static void test_nx_al_arena_realloc(void) {
    nx_al *al = nx_al_arena_new(2048); // 2KB
    TEST_ASSERT_TRUE(al != nx_null);

    const nx_usize len = 10;
    nx_i32 *arr = NX_AL_ALLOC(nx_i32, al, len);
    TEST_ASSERT_TRUE(arr != nx_null);

    for (nx_usize i = 0; i < len; ++i) {
        arr[i] = (nx_i32) (i * i);
    }

    const nx_usize new_len = 20;
    nx_i32 *tmp = NX_AL_REALLOC(nx_i32, al, arr, len, new_len);
    TEST_ASSERT_TRUE(tmp != nx_null);

    arr = tmp;

    for (nx_usize i = 0; i < len; ++i) {
        TEST_ASSERT_EQUAL_INT32(arr[i], (nx_i32) (i * i));
    }

    for (nx_usize i = len; i < new_len; ++i) {
        arr[i] = (nx_i32) (i * i * i);
    }

    for (nx_usize i = len; i < new_len; ++i) {
        TEST_ASSERT_EQUAL_INT32(arr[i], (nx_i32) (i * i * i));
    }

    NX_AL_DEALLOC(nx_i32, al, arr, new_len);
    nx_al_arena_drop(al);
}

static void test_nx_al_arena_reset(void) {
    nx_al *al = nx_al_arena_new(1024);
    TEST_ASSERT_TRUE(al != nx_null);

    nx_i32 *arr1 = NX_AL_ALLOC(nx_i32, al, 100);
    TEST_ASSERT_TRUE(arr1 != nx_null);

    nx_al_arena_stats stats = nx_al_arena_get_stats(al);
    TEST_ASSERT_TRUE(stats.used > 0);
    TEST_ASSERT_TRUE(stats.used <= stats.cap);

    nx_al_arena_reset(al);

    stats = nx_al_arena_get_stats(al);
    TEST_ASSERT_EQUAL_UINT(0, stats.used);
    TEST_ASSERT_EQUAL_UINT(1024, stats.available);

    nx_i32 *arr2 = NX_AL_ALLOC(nx_i32, al, 50);
    TEST_ASSERT_TRUE(arr2 != nx_null);

    TEST_ASSERT_EQUAL_PTR(arr1, arr2);

    nx_al_arena_drop(al);
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

    const nx_al_arena_stats stats2 = nx_al_arena_get_stats(al);
    TEST_ASSERT_TRUE(stats2.used > stats.used);
    TEST_ASSERT_EQUAL_UINT(capacity - stats2.used, stats2.available);

    nx_al_arena_drop(al);
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

    nx_al_arena_drop(al);
}

static void test_nx_al_arena_alignment(void) {
    nx_al *al = nx_al_arena_new(1024);
    TEST_ASSERT_TRUE(al != nx_null);

    void *p1 = nx_al_alloc(al, 1);
    void *p2 = nx_al_alloc(al, 1);
    void *p3 = nx_al_alloc(al, 1);

    TEST_ASSERT_TRUE(p1 != nx_null);
    TEST_ASSERT_TRUE(p2 != nx_null);
    TEST_ASSERT_TRUE(p3 != nx_null);

    TEST_ASSERT_EQUAL_UINT(0, ((nx_usize) p1) % 8);
    TEST_ASSERT_EQUAL_UINT(0, ((nx_usize) p2) % 8);
    TEST_ASSERT_EQUAL_UINT(0, ((nx_usize) p3) % 8);

    TEST_ASSERT_EQUAL_UINT(alignof(max_align_t), (nx_byte*) p2 - (nx_byte*) p1);
    TEST_ASSERT_EQUAL_UINT(alignof(max_align_t), (nx_byte*) p3 - (nx_byte*) p2);

    nx_al_arena_drop(al);
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
