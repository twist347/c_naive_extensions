#include "criterion/criterion.h"

#include "nx/data_structure/vec.h"

static nx_vec *vec_new_or_die(nx_usize elem_size) {
    nx_vec_res res = nx_vec_new(elem_size);
    if (!NX_RES_IS_OK(res)) {
        nx_status_fprintln(stderr, NX_RES_ERR(res));
        exit(EXIT_FAILURE);
    }
    return NX_RES_VAL(res);
}

static nx_vec *vec_new_len_or_die(nx_usize len, nx_usize elem_size) {
    nx_vec_res res = nx_vec_new_len(len, elem_size);
    if (!NX_RES_IS_OK(res)) {
        nx_status_fprintln(stderr, NX_RES_ERR(res));
        exit(EXIT_FAILURE);
    }
    return NX_RES_VAL(res);
}

static nx_vec *vec_new_cap_or_die(nx_usize cap, nx_usize elem_size) {
    nx_vec_res res = nx_vec_new_cap(cap, elem_size);
    if (!NX_RES_IS_OK(res)) {
        nx_status_fprintln(stderr, NX_RES_ERR(res));
        exit(EXIT_FAILURE);
    }
    return NX_RES_VAL(res);
}

Test(nx_vec_new, regular) {
    nx_vec_res res = NX_VEC_NEW(nx_i32);
    if (!NX_RES_IS_OK(res)) {
        nx_status_fprintln(stderr, NX_RES_ERR(res));
        exit(EXIT_FAILURE);
    }
    nx_vec *vec = NX_RES_VAL(res);

    cr_assert_eq(nx_vec_data(vec), nx_null);
    cr_assert_eq(nx_vec_len(vec), 0);
    cr_assert_eq(nx_vec_cap(vec), 0);
    cr_assert_eq(nx_vec_elem_size(vec), sizeof(nx_i32));

    nx_vec_drop(vec);
}

Test(nx_vec_new_len, regular) {
    nx_vec_res res = NX_VEC_NEW_LEN(nx_i32, 5);
    if (!NX_RES_IS_OK(res)) {
        nx_status_fprintln(stderr, NX_RES_ERR(res));
        exit(EXIT_FAILURE);
    }
    nx_vec *vec = NX_RES_VAL(res);

    cr_assert_neq(nx_vec_data(vec), nx_null);
    cr_assert_eq(nx_vec_len(vec), 5);
    cr_assert_eq(nx_vec_cap(vec), 5);
    cr_assert_eq(nx_vec_elem_size(vec), sizeof(nx_i32));

    for (nx_usize i = 0; i < nx_vec_len(vec); ++i) {
        cr_assert_eq(*NX_VEC_GET_AS_C(nx_i32, vec, i), 0);
    }

    nx_vec_drop(vec);
}

Test(nx_vec_new_cap, regular) {
    nx_vec_res res = NX_VEC_NEW_CAP(nx_i32, 5);
    if (!NX_RES_IS_OK(res)) {
        nx_status_fprintln(stderr, NX_RES_ERR(res));
        exit(EXIT_FAILURE);
    }
    nx_vec *vec = NX_RES_VAL(res);

    cr_assert_neq(nx_vec_data(vec), nx_null);
    cr_assert_eq(nx_vec_len(vec), 0);
    cr_assert_eq(nx_vec_cap(vec), 5);
    cr_assert_eq(nx_vec_elem_size(vec), sizeof(nx_i32));

    nx_vec_drop(vec);
}

Test(nx_vec_new_drop, regular) {
    nx_vec_res res = NX_VEC_NEW_LEN(nx_i32, 5);
    if (!NX_RES_IS_OK(res)) {
        nx_status_fprintln(stderr, NX_RES_ERR(res));
        exit(EXIT_FAILURE);
    }
    nx_vec *vec = NX_RES_VAL(res);

    nx_vec_drop(vec);
}

Test(nx_vec_new_drop, null) {
    nx_vec *vec = nx_null;

    nx_vec_drop(vec);
}