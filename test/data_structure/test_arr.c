#include "unity.h"

#include "nx/core/panic.h"
#include "nx/core/status.h"
#include "nx/core/util.h"
#include "nx/data_structure/arr.h"
#include "nx/mem/alloc_arena.h"
#include "nx/mem/alloc_libc.h"
#include "nx/mem/ptr.h"

void setUp(void) {
}

void tearDown(void) {
}

static nx_arr *arr_new_or_die(nx_usize len, nx_usize tsz) {
    nx_arr_res res = nx_arr_new_len(len, tsz);
    if (!NX_RES_IS_OK(res)) {
        NX_PANIC_MSG(nx_status_to_str(NX_RES_ERR(res)));
    }
    return NX_RES_UNWRAP(res);
}

/* ========== lifetime ========== */

static void test_nx_arr_new_p(void) {
    nx_al *arena = nx_al_arena_new(1024);

    nx_arr_res res = nx_arr_new_p(
        (nx_arr_params){
            .len = 5,
            .tsz = sizeof(nx_i32),
            .al = arena
        }
    );

    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_arr *arr = NX_RES_UNWRAP(res);
    TEST_ASSERT_NOT_NULL(arr);

    TEST_ASSERT_NOT_NULL(nx_arr_data(arr));
    TEST_ASSERT_EQUAL_UINT64(5, nx_arr_len(arr));
    TEST_ASSERT_EQUAL_UINT64(sizeof(nx_i32), nx_arr_tsz(arr));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        TEST_ASSERT_EQUAL_INT32(0, *NX_ARR_GET_AS_C(nx_i32, arr, i));
    }

    nx_arr_drop(arr);
    nx_al_arena_drop(arena);
}

static void test_nx_arr_new_len(void) {
    nx_arr_res res = NX_ARR_NEW_LEN(nx_i32, 5);

    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_arr *arr = NX_RES_UNWRAP(res);
    TEST_ASSERT_NOT_NULL(arr);

    TEST_ASSERT_NOT_NULL(nx_arr_data(arr));
    TEST_ASSERT_EQUAL_UINT64(5, nx_arr_len(arr));
    TEST_ASSERT_EQUAL_UINT64(sizeof(nx_i32), nx_arr_tsz(arr));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        TEST_ASSERT_EQUAL_INT32(0, *NX_ARR_GET_AS_C(nx_i32, arr, i));
    }

    nx_arr_drop(arr);
}

static void test_nx_arr_from_data(void) {
    const nx_i32 data[] = {1, 2, 3, 4, 5};

    nx_arr_res res = NX_ARR_FROM_DATA(nx_i32, data, NX_C_ARR_LEN(data));
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));
    nx_arr *arr = NX_RES_UNWRAP(res);

    TEST_ASSERT_NOT_NULL(nx_arr_data(arr));
    TEST_ASSERT_EQUAL_UINT64(NX_C_ARR_LEN(data), nx_arr_len(arr));
    TEST_ASSERT_EQUAL_UINT64(sizeof(nx_i32), nx_arr_tsz(arr));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        TEST_ASSERT_EQUAL_INT32(data[i], *NX_ARR_GET_AS_C(nx_i32, arr, i));
    }

    nx_arr_drop(arr);
}

static void test_nx_arr_new_len_empty(void) {
    nx_arr_res res = NX_ARR_NEW_LEN(nx_i32, 0);

    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_arr *arr = NX_RES_UNWRAP(res);
    TEST_ASSERT_NOT_NULL(arr);

    TEST_ASSERT_NULL(nx_arr_data(arr));
    TEST_ASSERT_EQUAL_UINT64(0, nx_arr_len(arr));
    TEST_ASSERT_EQUAL_UINT64(sizeof(nx_i32), nx_arr_tsz(arr));

    nx_arr_drop(arr);
}

static void test_nx_arr_new_len_out_of_memory(void) {
    typedef struct huge_type {
        nx_i64 arr[1024 * 1024];
    } huge_type;

    const nx_usize len = 1024 * 1024;

    nx_arr_res res = NX_ARR_NEW_LEN(huge_type, len);
    TEST_ASSERT_EQUAL_INT(NX_STATUS_OUT_OF_MEMORY, (int) NX_RES_ERR(res));
}

static void test_nx_arr_drop(void) {
    nx_arr_res res = NX_ARR_NEW_LEN(nx_i32, 5);

    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_arr *arr = NX_RES_UNWRAP(res);
    TEST_ASSERT_NOT_NULL(arr);

    TEST_ASSERT_NOT_NULL(nx_arr_data(arr));
    TEST_ASSERT_EQUAL_UINT64(5, nx_arr_len(arr));
    TEST_ASSERT_EQUAL_UINT64(sizeof(nx_i32), nx_arr_tsz(arr));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        TEST_ASSERT_EQUAL_INT32(0, *NX_ARR_GET_AS_C(nx_i32, arr, i));
    }

    nx_arr_drop(arr);
}

static void test_nx_arr_drop_null(void) {
    nx_arr *arr = nx_null;
    nx_arr_drop(arr);
}

/* ========== copy/move semantic ========== */
// TODO:

/* ========== info ========== */

static void test_nx_arr_len(void) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));
    nx_arr *empty = arr_new_or_die(0, sizeof(nx_i32));

    TEST_ASSERT_EQUAL_UINT64(5, nx_arr_len(arr));
    TEST_ASSERT_EQUAL_UINT64(0, nx_arr_len(empty));

    nx_arr_drop(empty);
    nx_arr_drop(arr);
}

static void test_nx_arr_empty(void) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));
    nx_arr *empty = arr_new_or_die(0, sizeof(nx_i32));

    TEST_ASSERT_FALSE(nx_arr_empty(arr));
    TEST_ASSERT_TRUE(nx_arr_empty(empty));

    nx_arr_drop(empty);
    nx_arr_drop(arr);
}

static void test_nx_arr_elem_size(void) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));
    nx_arr *empty = arr_new_or_die(0, sizeof(nx_i32));

    TEST_ASSERT_EQUAL_UINT64(sizeof(nx_i32), nx_arr_tsz(arr));
    TEST_ASSERT_EQUAL_UINT64(sizeof(nx_i32), nx_arr_tsz(empty));

    nx_arr_drop(empty);
    nx_arr_drop(arr);
}

/* ========== access ========== */

static void test_nx_arr_get(void) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        NX_ARR_SET_EXPR(int, arr, i, i * i);
    }

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        TEST_ASSERT_EQUAL_INT32((nx_i32) (i * i), *NX_ARR_GET_AS(nx_i32, arr, i));
    }

    nx_arr_drop(arr);
}

static void test_nx_arr_get_c(void) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        NX_ARR_SET_EXPR(int, arr, i, i * i);
    }

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        TEST_ASSERT_EQUAL_INT32((nx_i32) (i * i), *NX_ARR_GET_AS_C(nx_i32, arr, i));
    }

    nx_arr_drop(arr);
}

static void test_nx_arr_at(void) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        NX_ARR_SET_EXPR(int, arr, i, i * i);
    }

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        TEST_ASSERT_EQUAL_INT32((nx_i32) (i * i), *NX_ARR_AT_AS(nx_i32, arr, i));
    }

    TEST_ASSERT_NULL(NX_ARR_AT_AS(nx_i32, arr, 10));

    nx_arr_drop(arr);
}

static void test_nx_arr_at_c(void) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        NX_ARR_SET_EXPR(int, arr, i, i * i);
    }

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        TEST_ASSERT_EQUAL_INT32((nx_i32) (i * i), *NX_ARR_AT_AS_C(nx_i32, arr, i));
    }

    TEST_ASSERT_NULL(NX_ARR_AT_AS_C(nx_i32, arr, 10));

    nx_arr_drop(arr);
}

static void test_nx_arr_set(void) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        NX_ARR_SET_EXPR(int, arr, i, i * i);
    }

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        TEST_ASSERT_EQUAL_INT32((nx_i32) (i * i), *NX_ARR_GET_AS(nx_i32, arr, i));
    }

    nx_arr_drop(arr);
}

static void test_nx_arr_data(void) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));
    const nx_usize elem_size = nx_arr_tsz(arr);

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        NX_ARR_SET_EXPR(int, arr, i, i * i);
    }

    void *data = nx_arr_data(arr);

    *(nx_i32 *) nx_byte_offset(data, elem_size, 2) = 42;

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        if (i == 2) {
            TEST_ASSERT_EQUAL_INT32(42, *NX_ARR_GET_AS_C(nx_i32, arr, i));
            TEST_ASSERT_EQUAL_INT32(42, *(nx_i32 *) nx_byte_offset(data, elem_size, i));
        } else {
            TEST_ASSERT_EQUAL_INT32((nx_i32) (i * i), *NX_ARR_GET_AS_C(nx_i32, arr, i));
            TEST_ASSERT_EQUAL_INT32((nx_i32) (i * i), *(nx_i32 *) nx_byte_offset(data, elem_size, i));
        }
    }

    nx_arr_drop(arr);
}

static void test_nx_arr_data_c(void) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));
    const nx_usize elem_size = nx_arr_tsz(arr);

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        NX_ARR_SET_EXPR(int, arr, i, i * i);
    }

    const void *data = nx_arr_data_c(arr);

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        TEST_ASSERT_EQUAL_INT32((nx_i32) (i * i), *NX_ARR_GET_AS_C(nx_i32, arr, i));
        TEST_ASSERT_EQUAL_INT32((nx_i32) (i * i), *(nx_i32 *) nx_byte_offset_c(data, elem_size, i));
    }

    nx_arr_drop(arr);
}

/* ========== mods ========== */

static void test_nx_arr_swap(void) {
    const nx_usize len1 = 5, len2 = 3;

    nx_arr *arr1 = arr_new_or_die(len1, sizeof(nx_i32));
    nx_arr *arr2 = arr_new_or_die(len2, sizeof(nx_i32));

    for (nx_usize i = 0; i < len1; ++i) {
        NX_ARR_SET_EXPR(nx_i32, arr1, i, (nx_i32)(i * i));
    }

    for (nx_usize i = 0; i < len2; ++i) {
        NX_ARR_SET_EXPR(nx_i32, arr2, i, (nx_i32)(i * i * i));
    }

    nx_arr_swap(arr1, arr2);

    TEST_ASSERT_EQUAL_UINT64(len2, nx_arr_len(arr1));
    TEST_ASSERT_EQUAL_UINT64(len1, nx_arr_len(arr2));

    for (nx_usize i = 0; i < nx_arr_len(arr1); ++i) {
        TEST_ASSERT_EQUAL_INT32((nx_i32) (i * i * i), *NX_ARR_GET_AS_C(nx_i32, arr1, i));
    }

    for (nx_usize i = 0; i < nx_arr_len(arr2); ++i) {
        NX_ARR_SET_EXPR(nx_i32, arr2, i, (nx_i32)(i * i * i));
    }

    nx_arr_drop(arr1);
    nx_arr_drop(arr2);
}

/* ========== to span ========== */

static void test_nx_arr_to_span(void) {
    const nx_usize len = 5;
    nx_arr *arr = arr_new_or_die(len, sizeof(nx_i32));
    for (nx_usize i = 0; i < len; ++i) {
        NX_ARR_SET_EXPR(int, arr, i, i * i);
    }

    nx_span s = nx_arr_to_span(arr);

    TEST_ASSERT_EQUAL_UINT64(s.data, nx_arr_data(arr));
    TEST_ASSERT_EQUAL_UINT64(s.len, nx_arr_len(arr));
    TEST_ASSERT_EQUAL_UINT64(s.tsz, nx_arr_tsz(arr));

    NX_SPAN_SET(nx_i32, s, 0, 42);

    for (nx_usize i = 0; i < len; ++i) {
        TEST_ASSERT_EQUAL_INT32(*NX_SPAN_GET_AS_C(nx_i32, s, i), *NX_ARR_GET_AS_C(nx_i32, arr, i));
    }

    nx_arr_drop(arr);
}

static void test_nx_arr_to_cspan(void) {
    const nx_usize len = 5;
    nx_arr *arr = arr_new_or_die(len, sizeof(nx_i32));
    for (nx_usize i = 0; i < len; ++i) {
        NX_ARR_SET_EXPR(int, arr, i, i * i);
    }

    nx_cspan s = nx_arr_to_cspan(arr);

    TEST_ASSERT_EQUAL_UINT64(s.data, nx_arr_data(arr));
    TEST_ASSERT_EQUAL_UINT64(s.len, nx_arr_len(arr));
    TEST_ASSERT_EQUAL_UINT64(s.tsz, nx_arr_tsz(arr));

    for (nx_usize i = 0; i < len; ++i) {
        TEST_ASSERT_EQUAL_INT32(*NX_CSPAN_GET_AS_C(nx_i32, s, i), *NX_ARR_GET_AS_C(nx_i32, arr, i));
    }

    nx_arr_drop(arr);
}

/* ========== runner ========== */

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_nx_arr_new_p);
    RUN_TEST(test_nx_arr_new_len);
    RUN_TEST(test_nx_arr_from_data);
    RUN_TEST(test_nx_arr_new_len_empty);
    RUN_TEST(test_nx_arr_new_len_out_of_memory);
    RUN_TEST(test_nx_arr_drop);
    RUN_TEST(test_nx_arr_drop_null);

    RUN_TEST(test_nx_arr_len);
    RUN_TEST(test_nx_arr_empty);
    RUN_TEST(test_nx_arr_elem_size);

    RUN_TEST(test_nx_arr_get);
    RUN_TEST(test_nx_arr_get_c);
    RUN_TEST(test_nx_arr_at);
    RUN_TEST(test_nx_arr_at_c);
    RUN_TEST(test_nx_arr_set);
    RUN_TEST(test_nx_arr_data);
    RUN_TEST(test_nx_arr_data_c);

    RUN_TEST(test_nx_arr_swap);

    RUN_TEST(test_nx_arr_to_span);
    RUN_TEST(test_nx_arr_to_cspan);

    return UNITY_END();
}
