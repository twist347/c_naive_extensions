#include "unity.h"

#include "nx/mem/alloc_libc.h"

void setUp(void) {
}

void tearDown(void) {
}

static void test_nx_al_libc_alloc(void) {
    nx_al *al = nx_al_libc_new();

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

static void test_nx_al_libc_calloc(void) {
    nx_al *al = nx_al_libc_new();

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

static void test_nx_al_libc_realloc(void) {
    nx_al *al = nx_al_libc_new();

    const nx_usize len = 10;
    nx_i32 *arr = nx_al_alloc(al, len * sizeof(nx_i32));
    TEST_ASSERT_TRUE(arr != nx_null);

    for (nx_usize i = 0; i < len; ++i) {
        arr[i] = (nx_i32) (i * i);
    }

    for (nx_usize i = 0; i < len; ++i) {
        TEST_ASSERT_EQUAL_INT32(arr[i], (nx_i32)(i * i));
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


static void test_nx_default_allocator(void) {
    nx_al *g_al = nx_al_libc_default_g();

    const nx_usize len = 10;
    nx_i32 *arr = nx_al_alloc(g_al, len * sizeof(nx_i32));
    TEST_ASSERT_TRUE(arr != nx_null);

    for (nx_usize i = 0; i < len; ++i) {
        arr[i] = (nx_i32) (i * i);
    }

    for (nx_usize i = 0; i < len; ++i) {
        TEST_ASSERT_EQUAL_INT32(arr[i], (nx_i32)(i * i));
    }

    nx_al_dealloc(g_al, arr, len * sizeof(nx_i32));
}

int main(void) {
    RUN_TEST(test_nx_al_libc_alloc);
    RUN_TEST(test_nx_al_libc_calloc);
    RUN_TEST(test_nx_al_libc_realloc);
    RUN_TEST(test_nx_default_allocator);

    return UNITY_END();
}
