#include "unity.h"

#include "nx/core/assert.h"
#include "nx/core/status.h"
#include "nx/core/util.h"
#include "nx/ds/arr.h"
#include "nx/mem/alloc_arena.h"
#include "nx/mem/alloc_libc.h"
#include "nx/mem/ptr.h"

void setUp(void) {
}

void tearDown(void) {
}

static nx_Arr *arr_new_or_die(nx_usize len, nx_usize tsz) {
    nx_ArrRes res = nx_arr_new_len(len, tsz);
    if (!NX_RES_IS_OK(res)) {
        NX_PANIC_MSG(nx_status_to_str(NX_RES_ERR(res)));
    }
    return NX_RES_UNWRAP(res);
}

/* ========== lifetime ========== */

static void test_new_p(void) {
    nx_Al *arena = nx_al_arena_new(1024);

    nx_ArrRes res = nx_arr_new_p((nx_ArrParams){
        .len = 5, .tsz = sizeof(nx_i32), .al = arena
    });
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_Arr *arr = NX_RES_UNWRAP(res);
    TEST_ASSERT_EQUAL_UINT64(5, nx_arr_len(arr));
    TEST_ASSERT_EQUAL_UINT64(sizeof(nx_i32), nx_arr_tsz(arr));

    for (nx_usize i = 0; i < 5; ++i) {
        TEST_ASSERT_EQUAL_INT32(0, *NX_ARR_GET_AS_C(nx_i32, arr, i));
    }

    nx_arr_drop(arr);
    nx_al_arena_drop(arena);
}

static void test_new_len(void) {
    nx_ArrRes res = NX_ARR_NEW_LEN(nx_i32, 5);
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_Arr *arr = NX_RES_UNWRAP(res);
    TEST_ASSERT_EQUAL_UINT64(5, nx_arr_len(arr));

    for (nx_usize i = 0; i < 5; ++i) {
        TEST_ASSERT_EQUAL_INT32(0, *NX_ARR_GET_AS_C(nx_i32, arr, i));
    }

    nx_arr_drop(arr);
}

static void test_new_len_empty(void) {
    nx_ArrRes res = NX_ARR_NEW_LEN(nx_i32, 0);
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));

    nx_Arr *arr = NX_RES_UNWRAP(res);
    TEST_ASSERT_NULL(nx_arr_data(arr));
    TEST_ASSERT_EQUAL_UINT64(0, nx_arr_len(arr));
    TEST_ASSERT_TRUE(nx_arr_empty(arr));

    nx_arr_drop(arr);
}

static void test_from_data(void) {
    const nx_i32 data[] = {1, 2, 3, 4, 5};

    nx_ArrRes res = NX_ARR_FROM_DATA(nx_i32, data, NX_C_ARR_LEN(data));
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));
    nx_Arr *arr = NX_RES_UNWRAP(res);

    TEST_ASSERT_EQUAL_UINT64(5, nx_arr_len(arr));
    for (nx_usize i = 0; i < 5; ++i) {
        TEST_ASSERT_EQUAL_INT32(data[i], *NX_ARR_GET_AS_C(nx_i32, arr, i));
    }

    nx_arr_drop(arr);
}

static void test_from_data_no_double_init(void) {
    // from_data should NOT zero-init before copying.
    // We verify the data is correct — if it zero-inited, values would be 0.
    const nx_i32 data[] = {99, 88, 77};
    nx_ArrRes res = NX_ARR_FROM_DATA(nx_i32, data, 3);
    nx_Arr *arr = NX_RES_UNWRAP(res);

    TEST_ASSERT_EQUAL_INT32(99, *NX_ARR_GET_AS_C(nx_i32, arr, 0));
    TEST_ASSERT_EQUAL_INT32(88, *NX_ARR_GET_AS_C(nx_i32, arr, 1));
    TEST_ASSERT_EQUAL_INT32(77, *NX_ARR_GET_AS_C(nx_i32, arr, 2));

    nx_arr_drop(arr);
}

static void test_from_span(void) {
    nx_i32 data[] = {10, 20, 30};
    nx_CSpan s = NX_CSPAN_FROM_ARR(data);

    nx_ArrRes res = nx_arr_from_span(s);
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));
    nx_Arr *arr = NX_RES_UNWRAP(res);

    TEST_ASSERT_EQUAL_UINT64(3, nx_arr_len(arr));
    for (nx_usize i = 0; i < 3; ++i) {
        TEST_ASSERT_EQUAL_INT32(data[i], *NX_ARR_GET_AS_C(nx_i32, arr, i));
    }

    nx_arr_drop(arr);
}

static void test_drop_null(void) {
    nx_arr_drop(nx_null); // must not crash
}

/* ========== copy/move ========== */

static void test_copy(void) {
    const nx_i32 data[] = {1, 2, 3};
    nx_ArrRes res_src_ = NX_ARR_FROM_DATA(nx_i32, data, 3);
    nx_Arr *src = NX_RES_UNWRAP(res_src_);

    nx_ArrRes res = nx_arr_copy(src);
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));
    nx_Arr *dst = NX_RES_UNWRAP(res);

    TEST_ASSERT_EQUAL_UINT64(3, nx_arr_len(dst));
    TEST_ASSERT_TRUE(nx_arr_data(dst) != nx_arr_data(src)); // independent buffer

    for (nx_usize i = 0; i < 3; ++i) {
        TEST_ASSERT_EQUAL_INT32(*NX_ARR_GET_AS_C(nx_i32, src, i),
                                *NX_ARR_GET_AS_C(nx_i32, dst, i));
    }

    nx_arr_drop(dst);
    nx_arr_drop(src);
}

static void test_copy_empty(void) {
    nx_Arr *src = arr_new_or_die(0, sizeof(nx_i32));

    nx_ArrRes res = nx_arr_copy(src);
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));
    nx_Arr *dst = NX_RES_UNWRAP(res);

    TEST_ASSERT_EQUAL_UINT64(0, nx_arr_len(dst));
    TEST_ASSERT_TRUE(nx_arr_empty(dst));

    nx_arr_drop(dst);
    nx_arr_drop(src);
}

static void test_copy_a(void) {
    nx_Al *arena = nx_al_arena_new(4096);

    const nx_i32 data[] = {10, 20, 30};
    nx_ArrRes res_src_ = NX_ARR_FROM_DATA(nx_i32, data, 3);
    nx_Arr *src = NX_RES_UNWRAP(res_src_);

    nx_ArrRes res = nx_arr_copy_a(src, arena);
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));
    nx_Arr *dst = NX_RES_UNWRAP(res);

    for (nx_usize i = 0; i < 3; ++i) {
        TEST_ASSERT_EQUAL_INT32(data[i], *NX_ARR_GET_AS_C(nx_i32, dst, i));
    }

    nx_arr_drop(dst);
    nx_arr_drop(src);
    nx_al_arena_drop(arena);
}

static void test_move(void) {
    nx_Arr *src = arr_new_or_die(3, sizeof(nx_i32));
    NX_ARR_SET_EXPR(nx_i32, src, 0, 42);

    nx_Arr *dst = nx_arr_move(&src);
    TEST_ASSERT_NULL(src);
    TEST_ASSERT_EQUAL_INT32(42, *NX_ARR_GET_AS_C(nx_i32, dst, 0));

    nx_arr_drop(dst);
}

static void test_copy_assign_same_len(void) {
    nx_Arr *a = arr_new_or_die(3, sizeof(nx_i32));
    nx_Arr *b = arr_new_or_die(3, sizeof(nx_i32));

    for (nx_usize i = 0; i < 3; ++i) {
        NX_ARR_SET_EXPR(nx_i32, b, i, (nx_i32)(i + 10));
    }

    TEST_ASSERT_EQUAL(NX_STATUS_OK, nx_arr_copy_assign(a, b));

    for (nx_usize i = 0; i < 3; ++i) {
        TEST_ASSERT_EQUAL_INT32(*NX_ARR_GET_AS_C(nx_i32, b, i),
                                *NX_ARR_GET_AS_C(nx_i32, a, i));
    }

    nx_arr_drop(a);
    nx_arr_drop(b);
}

static void test_copy_assign_different_len(void) {
    nx_Arr *a = arr_new_or_die(2, sizeof(nx_i32));
    const nx_i32 data[] = {1, 2, 3, 4, 5};
    nx_ArrRes res_b_ = NX_ARR_FROM_DATA(nx_i32, data, 5);
    nx_Arr *b = NX_RES_UNWRAP(res_b_);

    TEST_ASSERT_EQUAL(NX_STATUS_OK, nx_arr_copy_assign(a, b));
    TEST_ASSERT_EQUAL_UINT64(5, nx_arr_len(a));

    for (nx_usize i = 0; i < 5; ++i) {
        TEST_ASSERT_EQUAL_INT32(data[i], *NX_ARR_GET_AS_C(nx_i32, a, i));
    }

    nx_arr_drop(a);
    nx_arr_drop(b);
}

static void test_copy_assign_self(void) {
    nx_Arr *a = arr_new_or_die(3, sizeof(nx_i32));
    NX_ARR_SET_EXPR(nx_i32, a, 0, 42);

    TEST_ASSERT_EQUAL(NX_STATUS_OK, nx_arr_copy_assign(a, a));
    TEST_ASSERT_EQUAL_INT32(42, *NX_ARR_GET_AS_C(nx_i32, a, 0));

    nx_arr_drop(a);
}

static void test_copy_assign_from_empty(void) {
    nx_Arr *a = arr_new_or_die(5, sizeof(nx_i32));
    nx_Arr *b = arr_new_or_die(0, sizeof(nx_i32));

    TEST_ASSERT_EQUAL(NX_STATUS_OK, nx_arr_copy_assign(a, b));
    TEST_ASSERT_EQUAL_UINT64(0, nx_arr_len(a));

    nx_arr_drop(a);
    nx_arr_drop(b);
}

static void test_move_assign(void) {
    nx_Arr *a = arr_new_or_die(2, sizeof(nx_i32));
    nx_Arr *b = arr_new_or_die(3, sizeof(nx_i32));
    NX_ARR_SET_EXPR(nx_i32, b, 0, 99);

    nx_arr_move_assign(a, b);
    TEST_ASSERT_EQUAL_UINT64(3, nx_arr_len(a));
    TEST_ASSERT_EQUAL_INT32(99, *NX_ARR_GET_AS_C(nx_i32, a, 0));
    TEST_ASSERT_EQUAL_UINT64(0, nx_arr_len(b));

    nx_arr_drop(a);
    nx_arr_drop(b);
}

/* ========== info ========== */

static void test_info(void) {
    nx_Arr *arr = arr_new_or_die(5, sizeof(nx_i32));
    nx_Arr *empty = arr_new_or_die(0, sizeof(nx_i32));

    TEST_ASSERT_EQUAL_UINT64(5, nx_arr_len(arr));
    TEST_ASSERT_EQUAL_UINT64(0, nx_arr_len(empty));
    TEST_ASSERT_FALSE(nx_arr_empty(arr));
    TEST_ASSERT_TRUE(nx_arr_empty(empty));
    TEST_ASSERT_EQUAL_UINT64(sizeof(nx_i32), nx_arr_tsz(arr));

    nx_arr_drop(empty);
    nx_arr_drop(arr);
}

/* ========== access ========== */

static void test_get_set(void) {
    nx_Arr *arr = arr_new_or_die(5, sizeof(nx_i32));

    for (nx_usize i = 0; i < 5; ++i) {
        NX_ARR_SET_EXPR(nx_i32, arr, i, (nx_i32)(i * i));
    }

    for (nx_usize i = 0; i < 5; ++i) {
        TEST_ASSERT_EQUAL_INT32((nx_i32)(i * i), *NX_ARR_GET_AS(nx_i32, arr, i));
        TEST_ASSERT_EQUAL_INT32((nx_i32)(i * i), *NX_ARR_GET_AS_C(nx_i32, arr, i));
    }

    nx_arr_drop(arr);
}

static void test_at(void) {
    nx_Arr *arr = arr_new_or_die(3, sizeof(nx_i32));
    NX_ARR_SET_EXPR(nx_i32, arr, 0, 42);

    TEST_ASSERT_NOT_NULL(NX_ARR_AT_AS(nx_i32, arr, 0));
    TEST_ASSERT_EQUAL_INT32(42, *NX_ARR_AT_AS(nx_i32, arr, 0));
    TEST_ASSERT_NULL(NX_ARR_AT_AS(nx_i32, arr, 10));
    TEST_ASSERT_NULL(NX_ARR_AT_AS_C(nx_i32, arr, 10));

    nx_arr_drop(arr);
}

static void test_data(void) {
    nx_Arr *arr = arr_new_or_die(3, sizeof(nx_i32));
    NX_ARR_SET_EXPR(nx_i32, arr, 1, 77);

    nx_i32 *raw = (nx_i32 *) nx_arr_data(arr);
    TEST_ASSERT_EQUAL_INT32(77, raw[1]);

    const nx_i32 *raw_c = (const nx_i32 *) nx_arr_data_c(arr);
    TEST_ASSERT_EQUAL_INT32(77, raw_c[1]);

    nx_arr_drop(arr);
}

/* ========== swap ========== */

static void test_swap(void) {
    const nx_i32 d1[] = {1, 2, 3};
    const nx_i32 d2[] = {10, 20};
    nx_ArrRes res_a_ = NX_ARR_FROM_DATA(nx_i32, d1, 3);
    nx_Arr *a = NX_RES_UNWRAP(res_a_);
    nx_ArrRes res_b_ = NX_ARR_FROM_DATA(nx_i32, d2, 2);
    nx_Arr *b = NX_RES_UNWRAP(res_b_);

    nx_arr_swap(a, b);

    TEST_ASSERT_EQUAL_UINT64(2, nx_arr_len(a));
    TEST_ASSERT_EQUAL_UINT64(3, nx_arr_len(b));
    TEST_ASSERT_EQUAL_INT32(10, *NX_ARR_GET_AS_C(nx_i32, a, 0));
    TEST_ASSERT_EQUAL_INT32(1, *NX_ARR_GET_AS_C(nx_i32, b, 0));

    nx_arr_drop(a);
    nx_arr_drop(b);
}

/* ========== to span ========== */

static void test_to_span(void) {
    nx_Arr *arr = arr_new_or_die(3, sizeof(nx_i32));
    NX_ARR_SET_EXPR(nx_i32, arr, 0, 42);

    nx_Span s = nx_arr_to_span(arr);
    TEST_ASSERT_EQUAL_PTR(nx_arr_data(arr), s.data);
    TEST_ASSERT_EQUAL_UINT64(3, s.len);

    NX_SPAN_SET(nx_i32, s, 1, 99);
    TEST_ASSERT_EQUAL_INT32(99, *NX_ARR_GET_AS_C(nx_i32, arr, 1));

    nx_arr_drop(arr);
}

static void test_to_cspan(void) {
    nx_Arr *arr = arr_new_or_die(3, sizeof(nx_i32));
    NX_ARR_SET_EXPR(nx_i32, arr, 2, 77);

    nx_CSpan s = nx_arr_to_cspan(arr);
    TEST_ASSERT_EQUAL_PTR(nx_arr_data_c(arr), s.data);
    TEST_ASSERT_EQUAL_INT32(77, *NX_CSPAN_GET_AS_C(nx_i32, s, 2));

    nx_arr_drop(arr);
}

int main(void) {
    UNITY_BEGIN();

    /* lifetime */
    RUN_TEST(test_new_p);
    RUN_TEST(test_new_len);
    RUN_TEST(test_new_len_empty);
    RUN_TEST(test_from_data);
    RUN_TEST(test_from_data_no_double_init);
    RUN_TEST(test_from_span);
    RUN_TEST(test_drop_null);

    /* copy/move */
    RUN_TEST(test_copy);
    RUN_TEST(test_copy_empty);
    RUN_TEST(test_copy_a);
    RUN_TEST(test_move);
    RUN_TEST(test_copy_assign_same_len);
    RUN_TEST(test_copy_assign_different_len);
    RUN_TEST(test_copy_assign_self);
    RUN_TEST(test_copy_assign_from_empty);
    RUN_TEST(test_move_assign);

    /* info */
    RUN_TEST(test_info);

    /* access */
    RUN_TEST(test_get_set);
    RUN_TEST(test_at);
    RUN_TEST(test_data);

    /* swap */
    RUN_TEST(test_swap);

    /* span */
    RUN_TEST(test_to_span);
    RUN_TEST(test_to_cspan);

    return UNITY_END();
}
