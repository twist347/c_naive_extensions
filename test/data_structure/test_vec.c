#include "unity.h"

#include <stdio.h>
#include <stdlib.h>

#include "nx/core/panic.h"
#include "nx/core/status.h"
#include "nx/core/util.h"
#include "nx/data_structure/vec.h"
#include "nx/mem/ptr.h"

static nx_vec *vec_new_or_die(nx_usize elem_size) {
    nx_vec_res res = nx_vec_new(elem_size);
    if (!NX_RES_IS_OK(res)) {
        NX_PANIC_MSG(nx_status_to_str(NX_RES_ERR(res)));
    }
    return NX_RES_UNWRAP(res);
}

static nx_vec *vec_new_len_or_die(nx_usize len, nx_usize elem_size) {
    nx_vec_res res = nx_vec_new_len(len, elem_size);
    if (!NX_RES_IS_OK(res)) {
        NX_PANIC_MSG(nx_status_to_str(NX_RES_ERR(res)));
    }
    return NX_RES_UNWRAP(res);
}

static nx_vec *vec_new_cap_or_die(nx_usize cap, nx_usize elem_size) {
    nx_vec_res res = nx_vec_new_cap(cap, elem_size);
    if (!NX_RES_IS_OK(res)) {
        NX_PANIC_MSG(nx_status_to_str(NX_RES_ERR(res)));
    }
    return NX_RES_UNWRAP(res);
}

void setUp(void) {
}

void tearDown(void) {
}

/* ========== lifetime ========== */

static void test_nx_vec_new(void) {
    nx_vec_res res = NX_VEC_NEW(nx_i32);
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_vec *vec = NX_RES_UNWRAP(res);

    TEST_ASSERT_NULL(nx_vec_data(vec));
    TEST_ASSERT_EQUAL_UINT64(0, nx_vec_len(vec));
    TEST_ASSERT_EQUAL_UINT64(0, nx_vec_cap(vec));
    TEST_ASSERT_EQUAL_UINT64(sizeof(nx_i32), nx_vec_tsz(vec));

    nx_vec_drop(vec);
}

static void test_nx_vec_new_len(void) {
    nx_vec_res res = NX_VEC_NEW_LEN(nx_i32, 5);
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_vec *vec = NX_RES_UNWRAP(res);

    TEST_ASSERT_NOT_NULL(nx_vec_data(vec));
    TEST_ASSERT_EQUAL_UINT64(5, nx_vec_len(vec));
    TEST_ASSERT_EQUAL_UINT64(5, nx_vec_cap(vec));
    TEST_ASSERT_EQUAL_UINT64(sizeof(nx_i32), nx_vec_tsz(vec));

    for (nx_usize i = 0; i < nx_vec_len(vec); ++i) {
        TEST_ASSERT_EQUAL_INT32(0, *NX_VEC_GET_AS_C(nx_i32, vec, i));
    }

    nx_vec_drop(vec);
}

static void test_nx_vec_new_cap(void) {
    nx_vec_res res = NX_VEC_NEW_CAP(nx_i32, 5);
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_vec *vec = NX_RES_UNWRAP(res);

    TEST_ASSERT_NOT_NULL(nx_vec_data(vec));
    TEST_ASSERT_EQUAL_UINT64(0, nx_vec_len(vec));
    TEST_ASSERT_EQUAL_UINT64(5, nx_vec_cap(vec));
    TEST_ASSERT_EQUAL_UINT64(sizeof(nx_i32), nx_vec_tsz(vec));

    nx_vec_drop(vec);
}

static void test_nx_vec_from_data(void) {
    const nx_i32 data[] = {1, 2, 3, 4, 5};

    nx_vec_res res = NX_VEC_FROM_DATA(nx_i32, data, NX_C_ARR_LEN(data));
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_vec *vec = NX_RES_UNWRAP(res);

    TEST_ASSERT_NOT_NULL(nx_vec_data(vec));
    TEST_ASSERT_EQUAL_UINT64(NX_C_ARR_LEN(data), nx_vec_len(vec));
    TEST_ASSERT_EQUAL_UINT64(NX_C_ARR_LEN(data), nx_vec_cap(vec));
    TEST_ASSERT_EQUAL_UINT64(sizeof(nx_i32), nx_vec_tsz(vec));

    for (nx_usize i = 0; i < nx_vec_len(vec); ++i) {
        TEST_ASSERT_EQUAL_INT32(data[i], *NX_VEC_GET_AS_C(nx_i32, vec, i));
    }

    nx_vec_drop(vec);
}


static void test_nx_vec_new_drop(void) {
    nx_vec_res res = NX_VEC_NEW_LEN(nx_i32, 5);
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_vec *vec = NX_RES_UNWRAP(res);
    nx_vec_drop(vec);
}

static void test_nx_vec_new_drop_null(void) {
    nx_vec *vec = nx_null;
    nx_vec_drop(vec);
}

/* ========== runner ========== */

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_nx_vec_new);
    RUN_TEST(test_nx_vec_new_len);
    RUN_TEST(test_nx_vec_new_cap);
    RUN_TEST(test_nx_vec_from_data);
    RUN_TEST(test_nx_vec_new_drop);
    RUN_TEST(test_nx_vec_new_drop_null);

    return UNITY_END();
}
