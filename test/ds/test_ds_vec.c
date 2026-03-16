#include "unity.h"

#include "nx/core/assert.h"
#include "nx/core/status.h"
#include "nx/core/util.h"
#include "nx/ds/vec.h"
#include "nx/mem/alloc_arena.h"
#include "nx/mem/ptr.h"

void setUp(void) {
}

void tearDown(void) {
}

static nx_Vec *vec_new_or_die(nx_usize tsz) {
    nx_VecRes res = nx_vec_new(tsz);
    if (!NX_RES_IS_OK(res)) {
        NX_PANIC_MSG(nx_status_to_str(NX_RES_ERR(res)));
    }
    return NX_RES_UNWRAP(res);
}

/* ========== lifetime ========== */

static void test_new(void) {
    nx_VecRes res = NX_VEC_NEW(nx_i32);
    TEST_ASSERT_TRUE(NX_RES_IS_OK(res));
    nx_Vec *v = NX_RES_UNWRAP(res);

    TEST_ASSERT_NULL(nx_vec_data(v));
    TEST_ASSERT_EQUAL_UINT64(0, nx_vec_len(v));
    TEST_ASSERT_EQUAL_UINT64(0, nx_vec_cap(v));
    TEST_ASSERT_TRUE(nx_vec_empty(v));

    nx_vec_drop(v);
}

static void test_new_len(void) {
    nx_VecRes res = NX_VEC_NEW_LEN(nx_i32, 5);
    nx_Vec *v = NX_RES_UNWRAP(res);

    TEST_ASSERT_EQUAL_UINT64(5, nx_vec_len(v));
    TEST_ASSERT_EQUAL_UINT64(5, nx_vec_cap(v));
    for (nx_usize i = 0; i < 5; ++i) {
        TEST_ASSERT_EQUAL_INT32(0, *NX_VEC_GET_AS_C(nx_i32, v, i));
    }

    nx_vec_drop(v);
}

static void test_new_cap(void) {
    nx_VecRes res = NX_VEC_NEW_CAP(nx_i32, 16);
    nx_Vec *v = NX_RES_UNWRAP(res);

    TEST_ASSERT_EQUAL_UINT64(0, nx_vec_len(v));
    TEST_ASSERT_EQUAL_UINT64(16, nx_vec_cap(v));

    nx_vec_drop(v);
}

static void test_from_data(void) {
    const nx_i32 data[] = {10, 20, 30};
    nx_VecRes res = NX_VEC_FROM_DATA(nx_i32, data, 3);
    nx_Vec *v = NX_RES_UNWRAP(res);

    TEST_ASSERT_EQUAL_UINT64(3, nx_vec_len(v));
    for (nx_usize i = 0; i < 3; ++i) {
        TEST_ASSERT_EQUAL_INT32(data[i], *NX_VEC_GET_AS_C(nx_i32, v, i));
    }

    nx_vec_drop(v);
}

static void test_from_span(void) {
    nx_i32 data[] = {1, 2, 3, 4};
    nx_CSpan s = NX_CSPAN_FROM_ARR(data);

    nx_VecRes res = nx_vec_from_span(s);
    nx_Vec *v = NX_RES_UNWRAP(res);

    TEST_ASSERT_EQUAL_UINT64(4, nx_vec_len(v));
    for (nx_usize i = 0; i < 4; ++i) {
        TEST_ASSERT_EQUAL_INT32(data[i], *NX_VEC_GET_AS_C(nx_i32, v, i));
    }

    nx_vec_drop(v);
}

static void test_drop_null(void) {
    nx_vec_drop(nx_null);
}

/* ========== copy/move ========== */

static void test_copy(void) {
    const nx_i32 data[] = {1, 2, 3};
    nx_VecRes res = NX_VEC_FROM_DATA(nx_i32, data, 3);
    nx_Vec *src = NX_RES_UNWRAP(res);

    nx_VecRes res2 = nx_vec_copy(src);
    nx_Vec *dst = NX_RES_UNWRAP(res2);

    TEST_ASSERT_EQUAL_UINT64(3, nx_vec_len(dst));
    TEST_ASSERT_TRUE(nx_vec_data(dst) != nx_vec_data(src));
    for (nx_usize i = 0; i < 3; ++i) {
        TEST_ASSERT_EQUAL_INT32(*NX_VEC_GET_AS_C(nx_i32, src, i),
                                *NX_VEC_GET_AS_C(nx_i32, dst, i));
    }

    nx_vec_drop(dst);
    nx_vec_drop(src);
}

static void test_move(void) {
    nx_Vec *src = vec_new_or_die(sizeof(nx_i32));
    NX_VEC_PUSH_EXPR(nx_i32, src, 42);

    nx_Vec *dst = nx_vec_move(&src);
    TEST_ASSERT_NULL(src);
    TEST_ASSERT_EQUAL_INT32(42, *NX_VEC_GET_AS_C(nx_i32, dst, 0));

    nx_vec_drop(dst);
}

static void test_copy_assign(void) {
    nx_Vec *a = vec_new_or_die(sizeof(nx_i32));
    NX_VEC_PUSH_EXPR(nx_i32, a, 1);

    const nx_i32 data[] = {10, 20, 30};
    nx_VecRes res = NX_VEC_FROM_DATA(nx_i32, data, 3);
    nx_Vec *b = NX_RES_UNWRAP(res);

    TEST_ASSERT_EQUAL(NX_STATUS_OK, nx_vec_copy_assign(a, b));
    TEST_ASSERT_EQUAL_UINT64(3, nx_vec_len(a));
    TEST_ASSERT_EQUAL_INT32(10, *NX_VEC_GET_AS_C(nx_i32, a, 0));

    nx_vec_drop(a);
    nx_vec_drop(b);
}

static void test_copy_assign_self(void) {
    nx_Vec *a = vec_new_or_die(sizeof(nx_i32));
    NX_VEC_PUSH_EXPR(nx_i32, a, 99);

    TEST_ASSERT_EQUAL(NX_STATUS_OK, nx_vec_copy_assign(a, a));
    TEST_ASSERT_EQUAL_INT32(99, *NX_VEC_GET_AS_C(nx_i32, a, 0));

    nx_vec_drop(a);
}

static void test_move_assign(void) {
    nx_Vec *a = vec_new_or_die(sizeof(nx_i32));
    NX_VEC_PUSH_EXPR(nx_i32, a, 1);

    nx_Vec *b = vec_new_or_die(sizeof(nx_i32));
    NX_VEC_PUSH_EXPR(nx_i32, b, 99);
    NX_VEC_PUSH_EXPR(nx_i32, b, 100);

    nx_vec_move_assign(a, b);
    TEST_ASSERT_EQUAL_UINT64(2, nx_vec_len(a));
    TEST_ASSERT_EQUAL_INT32(99, *NX_VEC_GET_AS_C(nx_i32, a, 0));
    TEST_ASSERT_EQUAL_UINT64(0, nx_vec_len(b));

    nx_vec_drop(a);
    nx_vec_drop(b);
}

/* ========== access ========== */

static void test_get_set(void) {
    nx_Vec *v = vec_new_or_die(sizeof(nx_i32));
    NX_VEC_PUSH_EXPR(nx_i32, v, 10);
    NX_VEC_PUSH_EXPR(nx_i32, v, 20);

    TEST_ASSERT_EQUAL_INT32(10, *NX_VEC_GET_AS(nx_i32, v, 0));
    TEST_ASSERT_EQUAL_INT32(20, *NX_VEC_GET_AS_C(nx_i32, v, 1));

    NX_VEC_SET(nx_i32, v, 0, 77);
    TEST_ASSERT_EQUAL_INT32(77, *NX_VEC_GET_AS(nx_i32, v, 0));

    nx_vec_drop(v);
}

static void test_at(void) {
    nx_Vec *v = vec_new_or_die(sizeof(nx_i32));
    NX_VEC_PUSH_EXPR(nx_i32, v, 42);

    TEST_ASSERT_EQUAL_INT32(42, *NX_VEC_AT_AS(nx_i32, v, 0));
    TEST_ASSERT_NULL(NX_VEC_AT_AS(nx_i32, v, 1));
    TEST_ASSERT_NULL(NX_VEC_AT_AS_C(nx_i32, v, 999));

    nx_vec_drop(v);
}

/* ========== push / pop ========== */

static void test_push_pop(void) {
    nx_Vec *v = vec_new_or_die(sizeof(nx_i32));

    for (nx_i32 i = 0; i < 100; ++i) {
        NX_VEC_PUSH_EXPR(nx_i32, v, i);
    }
    TEST_ASSERT_EQUAL_UINT64(100, nx_vec_len(v));

    for (nx_i32 i = 99; i >= 0; --i) {
        nx_i32 *val = NX_VEC_POP_PTR(nx_i32, v);
        TEST_ASSERT_EQUAL_INT32(i, *val);
    }
    TEST_ASSERT_EQUAL_UINT64(0, nx_vec_len(v));
    TEST_ASSERT_TRUE(nx_vec_empty(v));

    nx_vec_drop(v);
}

/* ========== insert / erase ========== */

static void test_insert_at_end(void) {
    nx_Vec *v = vec_new_or_die(sizeof(nx_i32));
    NX_VEC_PUSH_EXPR(nx_i32, v, 1);
    NX_VEC_PUSH_EXPR(nx_i32, v, 2);

    NX_VEC_INSERT_EXPR(nx_i32, v, 2, 3); // insert at end
    TEST_ASSERT_EQUAL_UINT64(3, nx_vec_len(v));
    TEST_ASSERT_EQUAL_INT32(3, *NX_VEC_GET_AS_C(nx_i32, v, 2));

    nx_vec_drop(v);
}

static void test_insert_at_beginning(void) {
    nx_Vec *v = vec_new_or_die(sizeof(nx_i32));
    NX_VEC_PUSH_EXPR(nx_i32, v, 2);
    NX_VEC_PUSH_EXPR(nx_i32, v, 3);

    NX_VEC_INSERT_EXPR(nx_i32, v, 0, 1); // insert at front
    TEST_ASSERT_EQUAL_UINT64(3, nx_vec_len(v));
    TEST_ASSERT_EQUAL_INT32(1, *NX_VEC_GET_AS_C(nx_i32, v, 0));
    TEST_ASSERT_EQUAL_INT32(2, *NX_VEC_GET_AS_C(nx_i32, v, 1));
    TEST_ASSERT_EQUAL_INT32(3, *NX_VEC_GET_AS_C(nx_i32, v, 2));

    nx_vec_drop(v);
}

static void test_erase(void) {
    nx_Vec *v = vec_new_or_die(sizeof(nx_i32));
    NX_VEC_PUSH_EXPR(nx_i32, v, 10);
    NX_VEC_PUSH_EXPR(nx_i32, v, 20);
    NX_VEC_PUSH_EXPR(nx_i32, v, 30);

    nx_vec_erase(v, 1); // remove 20
    TEST_ASSERT_EQUAL_UINT64(2, nx_vec_len(v));
    TEST_ASSERT_EQUAL_INT32(10, *NX_VEC_GET_AS_C(nx_i32, v, 0));
    TEST_ASSERT_EQUAL_INT32(30, *NX_VEC_GET_AS_C(nx_i32, v, 1));

    nx_vec_drop(v);
}

/* ========== extend ========== */

static void test_extend(void) {
    nx_Vec *v = vec_new_or_die(sizeof(nx_i32));
    NX_VEC_PUSH_EXPR(nx_i32, v, 1);

    nx_i32 more[] = {2, 3, 4, 5};
    nx_CSpan s = NX_CSPAN_FROM_ARR(more);

    TEST_ASSERT_EQUAL(NX_STATUS_OK, nx_vec_extend(v, s));
    TEST_ASSERT_EQUAL_UINT64(5, nx_vec_len(v));

    for (nx_usize i = 0; i < 5; ++i) {
        TEST_ASSERT_EQUAL_INT32((nx_i32)(i + 1), *NX_VEC_GET_AS_C(nx_i32, v, i));
    }

    nx_vec_drop(v);
}

static void test_extend_empty(void) {
    nx_Vec *v = vec_new_or_die(sizeof(nx_i32));
    NX_VEC_PUSH_EXPR(nx_i32, v, 42);

    nx_CSpan empty = nx_cspan_new(nx_null, 0, sizeof(nx_i32));
    TEST_ASSERT_EQUAL(NX_STATUS_OK, nx_vec_extend(v, empty));
    TEST_ASSERT_EQUAL_UINT64(1, nx_vec_len(v));

    nx_vec_drop(v);
}

/* ========== clear / reserve / resize / shrink ========== */

static void test_clear(void) {
    nx_Vec *v = vec_new_or_die(sizeof(nx_i32));
    for (nx_i32 i = 0; i < 10; ++i) {
        NX_VEC_PUSH_EXPR(nx_i32, v, i);
    }
    const nx_usize old_cap = nx_vec_cap(v);

    nx_vec_clear(v);
    TEST_ASSERT_EQUAL_UINT64(0, nx_vec_len(v));
    TEST_ASSERT_EQUAL_UINT64(old_cap, nx_vec_cap(v)); // cap unchanged

    nx_vec_drop(v);
}

static void test_reserve(void) {
    nx_Vec *v = vec_new_or_die(sizeof(nx_i32));

    TEST_ASSERT_EQUAL(NX_STATUS_OK, nx_vec_reserve(v, 100));
    TEST_ASSERT_TRUE(nx_vec_cap(v) >= 100);
    TEST_ASSERT_EQUAL_UINT64(0, nx_vec_len(v));

    nx_vec_drop(v);
}

static void test_resize_grow(void) {
    nx_Vec *v = vec_new_or_die(sizeof(nx_i32));
    NX_VEC_PUSH_EXPR(nx_i32, v, 42);

    TEST_ASSERT_EQUAL(NX_STATUS_OK, nx_vec_resize(v, 5));
    TEST_ASSERT_EQUAL_UINT64(5, nx_vec_len(v));
    TEST_ASSERT_EQUAL_INT32(42, *NX_VEC_GET_AS_C(nx_i32, v, 0));
    // new elements zero-initialized
    for (nx_usize i = 1; i < 5; ++i) {
        TEST_ASSERT_EQUAL_INT32(0, *NX_VEC_GET_AS_C(nx_i32, v, i));
    }

    nx_vec_drop(v);
}

static void test_resize_shrink(void) {
    nx_Vec *v = vec_new_or_die(sizeof(nx_i32));
    for (nx_i32 i = 0; i < 10; ++i) {
        NX_VEC_PUSH_EXPR(nx_i32, v, i);
    }

    TEST_ASSERT_EQUAL(NX_STATUS_OK, nx_vec_resize(v, 3));
    TEST_ASSERT_EQUAL_UINT64(3, nx_vec_len(v));
    TEST_ASSERT_EQUAL_INT32(0, *NX_VEC_GET_AS_C(nx_i32, v, 0));
    TEST_ASSERT_EQUAL_INT32(2, *NX_VEC_GET_AS_C(nx_i32, v, 2));

    nx_vec_drop(v);
}

static void test_shrink_to_fit(void) {
    nx_Vec *v = vec_new_or_die(sizeof(nx_i32));
    for (nx_i32 i = 0; i < 20; ++i) {
        NX_VEC_PUSH_EXPR(nx_i32, v, i);
    }
    TEST_ASSERT_TRUE(nx_vec_cap(v) > nx_vec_len(v));

    TEST_ASSERT_EQUAL(NX_STATUS_OK, nx_vec_shrink_to_fit(v));
    TEST_ASSERT_EQUAL_UINT64(nx_vec_len(v), nx_vec_cap(v));

    // data intact
    for (nx_i32 i = 0; i < 20; ++i) {
        TEST_ASSERT_EQUAL_INT32(i, *NX_VEC_GET_AS_C(nx_i32, v, i));
    }

    nx_vec_drop(v);
}

static void test_shrink_to_fit_empty(void) {
    nx_Vec *v = vec_new_or_die(sizeof(nx_i32));
    NX_VEC_PUSH_EXPR(nx_i32, v, 1);
    nx_vec_clear(v);

    TEST_ASSERT_EQUAL(NX_STATUS_OK, nx_vec_shrink_to_fit(v));
    TEST_ASSERT_EQUAL_UINT64(0, nx_vec_cap(v));
    TEST_ASSERT_NULL(nx_vec_data(v));

    nx_vec_drop(v);
}

/* ========== swap ========== */

static void test_swap(void) {
    nx_Vec *a = vec_new_or_die(sizeof(nx_i32));
    nx_Vec *b = vec_new_or_die(sizeof(nx_i32));

    NX_VEC_PUSH_EXPR(nx_i32, a, 1);
    NX_VEC_PUSH_EXPR(nx_i32, b, 10);
    NX_VEC_PUSH_EXPR(nx_i32, b, 20);

    nx_vec_swap(a, b);
    TEST_ASSERT_EQUAL_UINT64(2, nx_vec_len(a));
    TEST_ASSERT_EQUAL_UINT64(1, nx_vec_len(b));
    TEST_ASSERT_EQUAL_INT32(10, *NX_VEC_GET_AS_C(nx_i32, a, 0));
    TEST_ASSERT_EQUAL_INT32(1, *NX_VEC_GET_AS_C(nx_i32, b, 0));

    nx_vec_drop(a);
    nx_vec_drop(b);
}

/* ========== to span ========== */

static void test_to_span(void) {
    nx_Vec *v = vec_new_or_die(sizeof(nx_i32));
    NX_VEC_PUSH_EXPR(nx_i32, v, 42);

    nx_Span s = nx_vec_to_span(v);
    TEST_ASSERT_EQUAL_PTR(nx_vec_data(v), s.data);
    TEST_ASSERT_EQUAL_UINT64(1, s.len);

    NX_SPAN_SET(nx_i32, s, 0, 99);
    TEST_ASSERT_EQUAL_INT32(99, *NX_VEC_GET_AS_C(nx_i32, v, 0));

    nx_vec_drop(v);
}

static void test_to_cspan(void) {
    nx_Vec *v = vec_new_or_die(sizeof(nx_i32));
    NX_VEC_PUSH_EXPR(nx_i32, v, 77);

    nx_CSpan s = nx_vec_to_cspan(v);
    TEST_ASSERT_EQUAL_INT32(77, *NX_CSPAN_GET_AS_C(nx_i32, s, 0));

    nx_vec_drop(v);
}

/* ========== large struct ========== */

typedef struct {
    nx_i64 a, b, c, d;
} BigVal;

static void test_large_struct(void) {
    nx_Vec *v = vec_new_or_die(sizeof(BigVal));

    for (nx_i64 i = 0; i < 50; ++i) {
        BigVal bv = {.a = i, .b = i * 2, .c = i * 3, .d = i * 4};
        nx_vec_push(v, &bv);
    }
    TEST_ASSERT_EQUAL_UINT64(50, nx_vec_len(v));

    const BigVal *p = nx_vec_get_c(v, 25);
    TEST_ASSERT_EQUAL_INT64(25, p->a);
    TEST_ASSERT_EQUAL_INT64(100, p->d);

    nx_vec_drop(v);
}

int main(void) {
    UNITY_BEGIN();

    /* lifetime */
    RUN_TEST(test_new);
    RUN_TEST(test_new_len);
    RUN_TEST(test_new_cap);
    RUN_TEST(test_from_data);
    RUN_TEST(test_from_span);
    RUN_TEST(test_drop_null);

    /* copy/move */
    RUN_TEST(test_copy);
    RUN_TEST(test_move);
    RUN_TEST(test_copy_assign);
    RUN_TEST(test_copy_assign_self);
    RUN_TEST(test_move_assign);

    /* access */
    RUN_TEST(test_get_set);
    RUN_TEST(test_at);

    /* push/pop */
    RUN_TEST(test_push_pop);

    /* insert/erase */
    RUN_TEST(test_insert_at_end);
    RUN_TEST(test_insert_at_beginning);
    RUN_TEST(test_erase);

    /* extend */
    RUN_TEST(test_extend);
    RUN_TEST(test_extend_empty);

    /* clear/reserve/resize/shrink */
    RUN_TEST(test_clear);
    RUN_TEST(test_reserve);
    RUN_TEST(test_resize_grow);
    RUN_TEST(test_resize_shrink);
    RUN_TEST(test_shrink_to_fit);
    RUN_TEST(test_shrink_to_fit_empty);

    /* swap */
    RUN_TEST(test_swap);

    /* span */
    RUN_TEST(test_to_span);
    RUN_TEST(test_to_cspan);

    /* misc */
    RUN_TEST(test_large_struct);

    return UNITY_END();
}
