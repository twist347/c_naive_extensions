#include "unity.h"

#include "nx/mem/alloc_pool.h"

void setUp(void) {
}

void tearDown(void) {
}

static void test_nx_al_pool_alloc_basic(void) {
    nx_Al *al = nx_al_pool_new(sizeof(nx_i32), 16);
    TEST_ASSERT_TRUE(al != nx_null);

    nx_i32 *a = NX_AL_ALLOC(nx_i32, al, 1);
    TEST_ASSERT_TRUE(a != nx_null);
    *a = 42;
    TEST_ASSERT_EQUAL_INT32(42, *a);

    nx_i32 *b = NX_AL_ALLOC(nx_i32, al, 1);
    TEST_ASSERT_TRUE(b != nx_null);
    *b = 99;
    TEST_ASSERT_EQUAL_INT32(99, *b);

    // a should still be valid
    TEST_ASSERT_EQUAL_INT32(42, *a);

    // different blocks
    TEST_ASSERT_TRUE(a != b);

    NX_AL_DEALLOC(nx_i32, al, a, 1);
    NX_AL_DEALLOC(nx_i32, al, b, 1);
    nx_al_pool_drop(al);
}

static void test_nx_al_pool_alloc_all_blocks(void) {
    constexpr nx_usize count = 8;
    nx_Al *al = nx_al_pool_new(sizeof(nx_i64), count);
    TEST_ASSERT_TRUE(al != nx_null);

    nx_i64 *ptrs[count];

    // allocate all blocks
    for (nx_usize i = 0; i < count; ++i) {
        ptrs[i] = NX_AL_ALLOC(nx_i64, al, 1);
        TEST_ASSERT_TRUE(ptrs[i] != nx_null);
        *ptrs[i] = (nx_i64) (i * 100);
    }

    // pool should be exhausted
    nx_i64 *overflow = NX_AL_ALLOC(nx_i64, al, 1);
    TEST_ASSERT_TRUE(overflow == nx_null);

    // verify values
    for (nx_usize i = 0; i < count; ++i) {
        TEST_ASSERT_EQUAL_INT64((nx_i64) (i * 100), *ptrs[i]);
    }

    for (nx_usize i = 0; i < count; ++i) {
        NX_AL_DEALLOC(nx_i64, al, ptrs[i], 1);
    }

    nx_al_pool_drop(al);
}

static void test_nx_al_pool_dealloc_reuse(void) {
    nx_Al *al = nx_al_pool_new(sizeof(nx_i32), 1);
    TEST_ASSERT_TRUE(al != nx_null);

    // alloc single block
    nx_i32 *a = NX_AL_ALLOC(nx_i32, al, 1);
    TEST_ASSERT_TRUE(a != nx_null);

    // pool is now full
    TEST_ASSERT_TRUE(NX_AL_ALLOC(nx_i32, al, 1) == nx_null);

    // free it
    NX_AL_DEALLOC(nx_i32, al, a, 1);

    // should be able to alloc again
    nx_i32 *b = NX_AL_ALLOC(nx_i32, al, 1);
    TEST_ASSERT_TRUE(b != nx_null);
    *b = 77;
    TEST_ASSERT_EQUAL_INT32(77, *b);

    NX_AL_DEALLOC(nx_i32, al, b, 1);
    nx_al_pool_drop(al);
}

static void test_nx_al_pool_calloc(void) {
    nx_Al *al = nx_al_pool_new(sizeof(nx_i32) * 4, 4);
    TEST_ASSERT_TRUE(al != nx_null);

    // calloc should zero-init
    nx_i32 *arr = NX_AL_CALLOC(nx_i32, al, 4);
    TEST_ASSERT_TRUE(arr != nx_null);

    for (nx_usize i = 0; i < 4; ++i) {
        TEST_ASSERT_EQUAL_INT32(0, arr[i]);
    }

    NX_AL_DEALLOC(nx_i32, al, arr, 4);
    nx_al_pool_drop(al);
}

static void test_nx_al_pool_realloc_fallback(void) {
    // pool has no native realloc; nx_al_realloc should use the
    // alloc + memcpy + dealloc fallback
    nx_Al *al = nx_al_pool_new(sizeof(nx_i32), 4);
    TEST_ASSERT_TRUE(al != nx_null);

    nx_i32 *a = NX_AL_ALLOC(nx_i32, al, 1);
    TEST_ASSERT_TRUE(a != nx_null);
    *a = 123;

    // realloc same size -> should succeed via fallback
    nx_i32 *b = NX_AL_REALLOC(nx_i32, al, a, 1, 1);
    TEST_ASSERT_TRUE(b != nx_null);
    TEST_ASSERT_EQUAL_INT32(123, *b);

    NX_AL_DEALLOC(nx_i32, al, b, 1);
    nx_al_pool_drop(al);
}

static void test_nx_al_pool_oversize_request_fails(void) {
    // block_size will be aligned up to alignof(max_align_t);
    // request something definitely larger than one aligned block
    constexpr nx_usize bsz = sizeof(nx_i32);
    nx_Al *al = nx_al_pool_new(bsz, 4);
    TEST_ASSERT_TRUE(al != nx_null);

    nx_AlPoolStats stats = nx_al_pool_get_stats(al);

    // request more than the actual aligned block size
    void *big = nx_al_alloc(al, stats.block_size + 1);
    TEST_ASSERT_TRUE(big == nx_null);

    nx_al_pool_drop(al);
}

static void test_nx_al_pool_zero_size_alloc(void) {
    nx_Al *al = nx_al_pool_new(sizeof(nx_i32), 4);
    TEST_ASSERT_TRUE(al != nx_null);

    void *p = nx_al_alloc(al, 0);
    TEST_ASSERT_TRUE(p == nx_null);

    nx_al_pool_drop(al);
}

static void test_nx_al_pool_reset(void) {
    constexpr nx_usize count = 4;
    nx_Al *al = nx_al_pool_new(sizeof(nx_i32), count);
    TEST_ASSERT_TRUE(al != nx_null);

    // fill up
    for (nx_usize i = 0; i < count; ++i) {
        nx_i32 *p = NX_AL_ALLOC(nx_i32, al, 1);
        TEST_ASSERT_TRUE(p != nx_null);
    }

    // exhausted
    TEST_ASSERT_TRUE(NX_AL_ALLOC(nx_i32, al, 1) == nx_null);

    // reset
    nx_al_pool_reset(al);

    // all blocks available again
    for (nx_usize i = 0; i < count; ++i) {
        nx_i32 *p = NX_AL_ALLOC(nx_i32, al, 1);
        TEST_ASSERT_TRUE(p != nx_null);
        *p = (nx_i32) i;
    }

    nx_al_pool_drop(al);
}

static void test_nx_al_pool_stats(void) {
    constexpr nx_usize count = 8;
    nx_Al *al = nx_al_pool_new(sizeof(nx_i32), count);
    TEST_ASSERT_TRUE(al != nx_null);

    nx_AlPoolStats stats = nx_al_pool_get_stats(al);
    TEST_ASSERT_EQUAL_UINT64(count, stats.block_count);
    TEST_ASSERT_EQUAL_UINT64(0, stats.used);
    TEST_ASSERT_EQUAL_UINT64(count, stats.free);
    TEST_ASSERT_TRUE(stats.block_size >= sizeof(nx_i32));

    // alloc 3
    nx_i32 *a = NX_AL_ALLOC(nx_i32, al, 1);
    nx_i32 *b = NX_AL_ALLOC(nx_i32, al, 1);
    nx_i32 *c = NX_AL_ALLOC(nx_i32, al, 1);

    stats = nx_al_pool_get_stats(al);
    TEST_ASSERT_EQUAL_UINT64(3, stats.used);
    TEST_ASSERT_EQUAL_UINT64(count - 3, stats.free);

    // dealloc 1
    NX_AL_DEALLOC(nx_i32, al, b, 1);

    stats = nx_al_pool_get_stats(al);
    TEST_ASSERT_EQUAL_UINT64(2, stats.used);
    TEST_ASSERT_EQUAL_UINT64(count - 2, stats.free);

    NX_AL_DEALLOC(nx_i32, al, a, 1);
    NX_AL_DEALLOC(nx_i32, al, c, 1);
    nx_al_pool_drop(al);
}

static void test_nx_al_pool_fifo_order(void) {
    // first alloc should return the first block in the buffer
    nx_Al *al = nx_al_pool_new(sizeof(nx_i64), 4);
    TEST_ASSERT_TRUE(al != nx_null);

    nx_i64 *a = NX_AL_ALLOC(nx_i64, al, 1);
    nx_i64 *b = NX_AL_ALLOC(nx_i64, al, 1);

    TEST_ASSERT_TRUE(a != nx_null);
    TEST_ASSERT_TRUE(b != nx_null);

    // a should come before b in memory (sequential allocation)
    TEST_ASSERT_TRUE((nx_byte *) b > (nx_byte *) a);

    NX_AL_DEALLOC(nx_i64, al, a, 1);
    NX_AL_DEALLOC(nx_i64, al, b, 1);
    nx_al_pool_drop(al);
}

static void test_nx_al_pool_large_struct(void) {
    typedef struct {
        nx_i64 x;
        nx_i64 y;
        nx_f64 z;
        nx_char name[32];
    } BigStruct;

    nx_Al *al = nx_al_pool_new(sizeof(BigStruct), 4);
    TEST_ASSERT_TRUE(al != nx_null);

    BigStruct *s = NX_AL_ALLOC(BigStruct, al, 1);
    TEST_ASSERT_TRUE(s != nx_null);

    s->x = 10;
    s->y = 20;
    s->z = 3.14;
    TEST_ASSERT_EQUAL_INT64(10, s->x);
    TEST_ASSERT_EQUAL_INT64(20, s->y);

    NX_AL_DEALLOC(BigStruct, al, s, 1);
    nx_al_pool_drop(al);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_nx_al_pool_alloc_basic);
    RUN_TEST(test_nx_al_pool_alloc_all_blocks);
    RUN_TEST(test_nx_al_pool_dealloc_reuse);
    RUN_TEST(test_nx_al_pool_calloc);
    RUN_TEST(test_nx_al_pool_realloc_fallback);
    RUN_TEST(test_nx_al_pool_oversize_request_fails);
    RUN_TEST(test_nx_al_pool_zero_size_alloc);
    RUN_TEST(test_nx_al_pool_reset);
    RUN_TEST(test_nx_al_pool_stats);
    RUN_TEST(test_nx_al_pool_fifo_order);
    RUN_TEST(test_nx_al_pool_large_struct);

    return UNITY_END();
}
