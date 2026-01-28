#include "criterion/criterion.h"

#include <stdio.h>

#include "nx/data_structure/arr.h"

Test(A, B) {
    nx_arr *arr = NULL;
    const nx_status st = NX_ARR_MAKE(&arr, nx_i32, 5);
    cr_assert_eq(st, NX_STATUS_OK);

    const int val = 69;

    NX_ARR_SET_EXPR(int, arr, 0, 42);
    nx_arr_set(arr, 1, &val);

    cr_assert_eq(*NX_ARR_GET_AS_C(nx_i32, arr, 0), 42);
    cr_assert_eq(*NX_ARR_GET_AS_C(nx_i32, arr, 1), val);

    cr_assert_eq(nx_arr_len(arr), 5);

    nx_arr_drop(arr);
}
