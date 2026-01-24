#include "criterion/criterion.h"

#include <stdio.h>

#include "nx/data_structure/arr.h"

Test(A, B) {
    nx_arr *arr = NULL;
    const nx_status st = NX_ARR_CREATE(&arr, int, 5);
    cr_assert_eq(st, NX_STATUS_OK);
    printf("%s\n", nx_status_to_str(st));

    NX_ARR_SET(int, arr, 0, 42);

    printf("%d\n", *NX_ARR_GET_PTR_C(int, arr, 0));

    nx_arr_destroy(arr);
}
