#include "unity.h"

#include "nx/mem/alloc_libc.h"

void setUp(void) {
}

void tearDown(void) {
}

static void test_nx_al_libc_alloc(void) {
    nx_al *al = nx_al_libc_new();

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
    nx_al_libc_drop(al);
}

static void test_nx_al_libc_calloc(void) {
    nx_al *al = nx_al_libc_new();

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
    nx_al_libc_drop(al);
}

static void test_nx_al_libc_realloc(void) {
    nx_al *al = nx_al_libc_new();

    const nx_usize len = 10;
    nx_i32 *arr = NX_AL_ALLOC(nx_i32, al, len);
    TEST_ASSERT_TRUE(arr != nx_null);

    for (nx_usize i = 0; i < len; ++i) {
        arr[i] = (nx_i32) (i * i);
    }

    for (nx_usize i = 0; i < len; ++i) {
        TEST_ASSERT_EQUAL_INT32(arr[i], (nx_i32) (i * i));
    }

    const nx_usize new_len = 20;

    nx_i32 *tmp = NX_AL_REALLOC(nx_i32, al, arr, len, new_len);
    TEST_ASSERT_TRUE(tmp != nx_null);

    arr = tmp;

    for (nx_usize i = 0; i < len; ++i) {
        TEST_ASSERT_EQUAL_INT32(arr[i], (nx_i32)(i * i));
    }

    for (nx_usize i = len; i < new_len; ++i) {
        arr[i] = (nx_i32) (i * i * i);
    }

    for (nx_usize i = len; i < new_len; ++i) {
        TEST_ASSERT_EQUAL_INT32(arr[i], (nx_i32) (i * i * i));
    }

    NX_AL_DEALLOC(nx_i32, al, arr, new_len);
    nx_al_libc_drop(al);
}


static void test_nx_default_allocator(void) {
    nx_al *g_al = nx_al_libc_default_g();

    const nx_usize len = 10;
    nx_i32 *arr = NX_AL_ALLOC(nx_i32, g_al, len);
    TEST_ASSERT_TRUE(arr != nx_null);

    for (nx_usize i = 0; i < len; ++i) {
        arr[i] = (nx_i32) (i * i);
    }

    for (nx_usize i = 0; i < len; ++i) {
        TEST_ASSERT_EQUAL_INT32(arr[i], (nx_i32) (i * i));
    }

    NX_AL_DEALLOC(nx_i32, g_al, arr, len);
}

int main(void) {
    RUN_TEST(test_nx_al_libc_alloc);
    RUN_TEST(test_nx_al_libc_calloc);
    RUN_TEST(test_nx_al_libc_realloc);
    RUN_TEST(test_nx_default_allocator);

    return UNITY_END();
}
