#include "unity.h"

#include <stdio.h>

#include "nx/core/panic.h"
#include "nx/core/status.h"
#include "nx/data_structure/arr.h"
#include "nx/mem/ptr.h"

static nx_arr *arr_new_or_die(nx_usize len, nx_usize elem_size) {
    nx_arr_res res = nx_arr_new_len(len, elem_size);
    if (!NX_RES_IS_OK(res)) {
        NX_PANIC_MSG(nx_status_to_str(NX_RES_ERR(res)));
    }
    return NX_RES_VAL(res);
}

void setUp(void) {
}

void tearDown(void) {
}

/* ========== lifetime ========== */

static void test_nx_arr_new_p_regular(void) {
    nx_arr_res res = nx_arr_new_p((nx_arr_params){.len = 5, .tsz = sizeof(nx_i32)});

    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_arr *arr = NX_RES_VAL(res);
    TEST_ASSERT_NOT_NULL(arr);

    TEST_ASSERT_NOT_NULL(nx_arr_data(arr));
    TEST_ASSERT_EQUAL_UINT64(5, (uint64_t) nx_arr_len(arr));
    TEST_ASSERT_EQUAL_UINT64(sizeof(nx_i32), (uint64_t) nx_arr_tsz(arr));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        TEST_ASSERT_EQUAL_INT32(0, *NX_ARR_GET_AS_C(nx_i32, arr, i));
    }

    nx_arr_drop(arr);
}

static void test_nx_arr_new_len_regular(void) {
    nx_arr_res res = NX_ARR_NEW_LEN(nx_i32, 5);

    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_arr *arr = NX_RES_VAL(res);
    TEST_ASSERT_NOT_NULL(arr);

    TEST_ASSERT_NOT_NULL(nx_arr_data(arr));
    TEST_ASSERT_EQUAL_UINT64(5, (uint64_t) nx_arr_len(arr));
    TEST_ASSERT_EQUAL_UINT64(sizeof(nx_i32), (uint64_t) nx_arr_tsz(arr));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        TEST_ASSERT_EQUAL_INT32(0, *NX_ARR_GET_AS_C(nx_i32, arr, i));
    }

    nx_arr_drop(arr);
}

static void test_nx_arr_new_len_empty(void) {
    nx_arr_res res = NX_ARR_NEW_LEN(nx_i32, 0);

    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_arr *arr = NX_RES_VAL(res);
    TEST_ASSERT_NOT_NULL(arr);

    TEST_ASSERT_NULL(nx_arr_data(arr));
    TEST_ASSERT_EQUAL_UINT64(0, (uint64_t) nx_arr_len(arr));
    TEST_ASSERT_EQUAL_UINT64(sizeof(nx_i32), (uint64_t) nx_arr_tsz(arr));

    nx_arr_drop(arr);
}

static void test_nx_arr_new_len_out_of_memory(void) {
    typedef struct huge_type {
        nx_i64 arr[1024 * 1024];
    } huge_type;

    const nx_usize len = 1024 * 1024;

    nx_arr_res res = NX_ARR_NEW_LEN(huge_type, len);
    TEST_ASSERT_EQUAL_INT((int) NX_STATUS_OUT_OF_MEMORY, (int) NX_RES_ERR(res));
}

static void test_nx_arr_drop_regular(void) {
    nx_arr_res res = NX_ARR_NEW_LEN(nx_i32, 5);

    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_arr *arr = NX_RES_VAL(res);
    TEST_ASSERT_NOT_NULL(arr);

    TEST_ASSERT_NOT_NULL(nx_arr_data(arr));
    TEST_ASSERT_EQUAL_UINT64(5, (uint64_t) nx_arr_len(arr));
    TEST_ASSERT_EQUAL_UINT64(sizeof(nx_i32), (uint64_t) nx_arr_tsz(arr));

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

static void test_nx_arr_len_regular(void) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));
    nx_arr *empty = arr_new_or_die(0, sizeof(nx_i32));

    TEST_ASSERT_EQUAL_UINT64(5, (uint64_t) nx_arr_len(arr));
    TEST_ASSERT_EQUAL_UINT64(0, (uint64_t) nx_arr_len(empty));

    nx_arr_drop(empty);
    nx_arr_drop(arr);
}

static void test_nx_arr_empty_regular(void) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));
    nx_arr *empty = arr_new_or_die(0, sizeof(nx_i32));

    TEST_ASSERT_FALSE(nx_arr_empty(arr));
    TEST_ASSERT_TRUE(nx_arr_empty(empty));

    nx_arr_drop(empty);
    nx_arr_drop(arr);
}

static void test_nx_arr_elem_size_regular(void) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));
    nx_arr *empty = arr_new_or_die(0, sizeof(nx_i32));

    TEST_ASSERT_EQUAL_UINT64(sizeof(nx_i32), (uint64_t) nx_arr_tsz(arr));
    TEST_ASSERT_EQUAL_UINT64(sizeof(nx_i32), (uint64_t) nx_arr_tsz(empty));

    nx_arr_drop(empty);
    nx_arr_drop(arr);
}

/* ========== access ========== */

static void test_nx_arr_get_regular(void) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        NX_ARR_SET_EXPR(int, arr, i, i * i);
    }

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        TEST_ASSERT_EQUAL_INT32((nx_i32) (i * i), *NX_ARR_GET_AS(nx_i32, arr, i));
    }

    nx_arr_drop(arr);
}

static void test_nx_arr_get_c_regular(void) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        NX_ARR_SET_EXPR(int, arr, i, i * i);
    }

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        TEST_ASSERT_EQUAL_INT32((nx_i32) (i * i), *NX_ARR_GET_AS_C(nx_i32, arr, i));
    }

    nx_arr_drop(arr);
}

static void test_nx_arr_set_regular(void) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        NX_ARR_SET_EXPR(int, arr, i, i * i);
    }

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        TEST_ASSERT_EQUAL_INT32((nx_i32) (i * i), *NX_ARR_GET_AS(nx_i32, arr, i));
    }

    nx_arr_drop(arr);
}

static void test_nx_arr_data_regular(void) {
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

static void test_nx_arr_data_c_regular(void) {
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

static void test_nx_arr_swap_regular(void) {
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

    TEST_ASSERT_EQUAL_UINT64(len2, (uint64_t) nx_arr_len(arr1));
    TEST_ASSERT_EQUAL_UINT64(len1, (uint64_t) nx_arr_len(arr2));

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

static void test_nx_arr_to_span_regular(void) {
    /* TODO */
}

static void test_nx_arr_to_cspan_regular(void) {
    /* TODO */
}

/* ========== runner ========== */

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_nx_arr_new_p_regular);
    RUN_TEST(test_nx_arr_new_len_regular);
    RUN_TEST(test_nx_arr_new_len_empty);
    RUN_TEST(test_nx_arr_new_len_out_of_memory);
    RUN_TEST(test_nx_arr_drop_regular);
    RUN_TEST(test_nx_arr_drop_null);

    RUN_TEST(test_nx_arr_len_regular);
    RUN_TEST(test_nx_arr_empty_regular);
    RUN_TEST(test_nx_arr_elem_size_regular);

    RUN_TEST(test_nx_arr_get_regular);
    RUN_TEST(test_nx_arr_get_c_regular);
    RUN_TEST(test_nx_arr_set_regular);
    RUN_TEST(test_nx_arr_data_regular);
    RUN_TEST(test_nx_arr_data_c_regular);

    RUN_TEST(test_nx_arr_swap_regular);

    RUN_TEST(test_nx_arr_to_span_regular);
    RUN_TEST(test_nx_arr_to_cspan_regular);

    return UNITY_END();
}
