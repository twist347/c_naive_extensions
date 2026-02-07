#include "criterion/criterion.h"

#include <stdio.h>

#include "nx/data_structure/arr.h"
#include "nx/mem/ptr.h"

static nx_arr *arr_new_or_die(nx_usize len, nx_usize elem_size) {
    nx_arr_res res = nx_arr_new_len(len, elem_size);
    if (!NX_RES_IS_OK(res)) {
        nx_status_fprintln(stderr, NX_RES_ERR(res));
        exit(EXIT_FAILURE);
    }
    return NX_RES_VAL(res);
}

/* ---------- lifetime ---------- */

Test(nx_arr_new_p, regular) {
    nx_arr_res res = nx_arr_new_p((nx_arr_params){.len = 5, .tsz = sizeof(nx_i32)});
    cr_assert(NX_RES_IS_OK(res));
    nx_arr *arr = NX_RES_VAL(res);
    cr_assert_neq(arr, nx_null);

    cr_assert_neq(nx_arr_data(arr), nx_null);
    cr_assert_eq(nx_arr_len(arr), 5);
    cr_assert_eq(nx_arr_tsz(arr), sizeof(nx_i32));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        cr_assert_eq(*NX_ARR_GET_AS_C(nx_i32, arr, i), 0);
    }

    nx_arr_drop(arr);
}

Test(nx_arr_new_len, regular) {
    nx_arr_res res = NX_ARR_NEW_LEN(nx_i32, 5);
    cr_assert(NX_RES_IS_OK(res));
    nx_arr *arr = NX_RES_VAL(res);
    cr_assert_neq(arr, nx_null);

    cr_assert_neq(nx_arr_data(arr), nx_null);
    cr_assert_eq(nx_arr_len(arr), 5);
    cr_assert_eq(nx_arr_tsz(arr), sizeof(nx_i32));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        cr_assert_eq(*NX_ARR_GET_AS_C(nx_i32, arr, i), 0);
    }

    nx_arr_drop(arr);
}

Test(nx_arr_new_len, empty) {
    nx_arr_res res = NX_ARR_NEW_LEN(nx_i32, 0);
    cr_assert(NX_RES_IS_OK(res));
    nx_arr *arr = NX_RES_VAL(res);
    cr_assert_neq(arr, nx_null);

    cr_assert_eq(nx_arr_data(arr), nx_null);
    cr_assert_eq(nx_arr_len(arr), 0);
    cr_assert_eq(nx_arr_tsz(arr), sizeof(nx_i32));

    nx_arr_drop(arr);
}

Test(nx_arr_new_len, out_of_memory) {
    typedef struct huge_type {
        nx_i64 arr[1024 * 1024];
    } huge_type;

    const nx_usize len = 1024 * 1024;

    nx_arr_res res = NX_ARR_NEW_LEN(huge_type, len);
    cr_assert_eq(NX_RES_ERR(res), NX_STATUS_OUT_OF_MEMORY);
}

Test(nx_arr_drop, regular) {
    nx_arr_res res = NX_ARR_NEW_LEN(nx_i32, 5);
    cr_assert(NX_RES_IS_OK(res));
    nx_arr *arr = NX_RES_VAL(res);
    cr_assert_neq(arr, nx_null);

    cr_assert_neq(nx_arr_data(arr), nx_null);
    cr_assert_eq(nx_arr_len(arr), 5);
    cr_assert_eq(nx_arr_tsz(arr), sizeof(nx_i32));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        cr_assert_eq(*NX_ARR_GET_AS_C(nx_i32, arr, i), 0);
    }

    nx_arr_drop(arr);
}

Test(nx_arr_drop, null) {
    nx_arr *arr = nx_null;

    nx_arr_drop(arr);
}

/* ---------- copy/move semantic ---------- */

// TODO:

/* ---------- info ---------- */

Test(nx_arr_len, regular) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));
    nx_arr *empty = arr_new_or_die(0, sizeof(nx_i32));

    cr_assert_eq(nx_arr_len(arr), 5);
    cr_assert_eq(nx_arr_len(empty), 0);

    nx_arr_drop(empty);
    nx_arr_drop(arr);
}

Test(nx_arr_empty, regular) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));
    nx_arr *empty = arr_new_or_die(0, sizeof(nx_i32));

    cr_assert_not(nx_arr_empty(arr));
    cr_assert(nx_arr_empty(empty));

    nx_arr_drop(empty);
    nx_arr_drop(arr);
}

Test(nx_arr_elem_size, regular) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));
    nx_arr *empty = arr_new_or_die(0, sizeof(nx_i32));

    cr_assert_eq(nx_arr_tsz(arr), sizeof(nx_i32));
    cr_assert_eq(nx_arr_tsz(empty), sizeof(nx_i32));

    nx_arr_drop(empty);
    nx_arr_drop(arr);
}

/* ---------- access ---------- */

Test(nx_arr_get, regular) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        NX_ARR_SET_EXPR(int, arr, i, i * i);
    }

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        cr_assert_eq(*NX_ARR_GET_AS(nx_i32, arr, i), (nx_i32) (i * i));
    }

    nx_arr_drop(arr);
}

Test(nx_arr_get_c, regular) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        NX_ARR_SET_EXPR(int, arr, i, i * i);
    }

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        cr_assert_eq(*NX_ARR_GET_AS_C(nx_i32, arr, i), (nx_i32) (i * i));
    }

    nx_arr_drop(arr);
}

Test(nx_arr_set, regular) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        NX_ARR_SET_EXPR(int, arr, i, i * i);
    }

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        cr_assert_eq(*NX_ARR_GET_AS(nx_i32, arr, i), (nx_i32) (i * i));
    }

    nx_arr_drop(arr);
}

Test(nx_arr_data, regular) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));
    const nx_usize elem_size = nx_arr_tsz(arr);

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        NX_ARR_SET_EXPR(int, arr, i, i * i);
    }

    void *data = nx_arr_data(arr);

    *(nx_i32 *) nx_byte_offset(data, elem_size, 2) = 42;

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        if (i == 2) {
            cr_assert_eq(*NX_ARR_GET_AS_C(nx_i32, arr, i), 42);
            cr_assert_eq(*(nx_i32 *) nx_byte_offset(data, elem_size, i), 42);
        } else {
            cr_assert_eq(*NX_ARR_GET_AS_C(nx_i32, arr, i), (nx_i32) (i * i));
            cr_assert_eq(*(nx_i32 *) nx_byte_offset(data, elem_size, i), (nx_i32) (i * i));
        }
    }

    nx_arr_drop(arr);
}

Test(nx_arr_data_c, regular) {
    nx_arr *arr = arr_new_or_die(5, sizeof(nx_i32));
    const nx_usize elem_size = nx_arr_tsz(arr);

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        NX_ARR_SET_EXPR(int, arr, i, i * i);
    }

    const void *data = nx_arr_data_c(arr);

    for (nx_usize i = 0; i < nx_arr_len(arr); ++i) {
        cr_assert_eq(*NX_ARR_GET_AS_C(nx_i32, arr, i), (nx_i32) (i * i));
        cr_assert_eq(*(nx_i32 *) nx_byte_offset_c(data, elem_size, i), (nx_i32) (i * i));
    }

    nx_arr_drop(arr);
}

/* ---------- mods ---------- */


Test(nx_arr_swap, regular) {
    const nx_usize len1 = 5, len2 = 3;

    nx_arr *arr1 = arr_new_or_die(len1, sizeof(nx_i32));
    nx_arr *arr2 = arr_new_or_die(len2, sizeof(nx_i32));

    for (nx_usize i = 0; i < len1; ++i) {
        NX_ARR_SET_EXPR(nx_i32, arr1, i, (nx_i32) (i * i));
    }

    for (nx_usize i = 0; i < len2; ++i) {
        NX_ARR_SET_EXPR(nx_i32, arr2, i, (nx_i32) (i * i * i));
    }

    nx_arr_swap(arr1, arr2);

    cr_assert_eq(nx_arr_len(arr1), len2);
    cr_assert_eq(nx_arr_len(arr2), len1);

    for (nx_usize i = 0; i < nx_arr_len(arr1); ++i) {
        cr_assert_eq(*NX_ARR_GET_AS_C(nx_i32, arr1, i), (nx_i32) (i * i * i));
    }

    for (nx_usize i = 0; i < nx_arr_len(arr2); ++i) {
        NX_ARR_SET_EXPR(nx_i32, arr2, i, (nx_i32) (i * i * i));
    }

    nx_arr_drop(arr1);
    nx_arr_drop(arr2);
}

/* ---------- to span ---------- */

Test(nx_arr_to_span, regular) {
}

Test(nx_arr_to_cspan, regular) {
}
