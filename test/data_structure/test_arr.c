#include "criterion/criterion.h"

#include <stdio.h>

#include "nx/data_structure/arr.h"

Test(A, B) {
    nx_arr_res res = NX_ARR_NEW_LEN(int, 5);
    if (!NX_RES_IS_OK(res)) {
        nx_status_fprint(stderr, NX_RES_GET_ERR(res));
        return;
    }

    nx_arr *arr = NX_RES_GET_VAL(res);

    const int val = 69;

    NX_ARR_SET_EXPR(int, arr, 0, 42);
    nx_arr_set(arr, 1, &val);

    cr_assert_eq(*NX_ARR_GET_AS_C(nx_i32, arr, 0), 42);
    cr_assert_eq(*NX_ARR_GET_AS_C(nx_i32, arr, 1), val);

    cr_assert_eq(nx_arr_len(arr), 5);

    nx_arr_drop(arr);
}
