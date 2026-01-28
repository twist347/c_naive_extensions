#include "nx/data_structure/arr.h"
#include "nx/core/print.h"
#include "nx/core/span.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    nx_arr *arr = nx_null;

    const nx_status st = NX_ARR_NEW(&arr, nx_i32, 5);
    if (st != NX_STATUS_OK) {
        nx_status_fprint(stderr, st);
        return EXIT_FAILURE;
    }

    const nx_usize len = nx_arr_len(arr);

    printf("len=%zu\n", len);
    printf("is empty=%d\n", nx_arr_empty(arr));
    printf("elem size=%zu\n", nx_arr_elem_size(arr));

    for (nx_usize i = 0; i < len; ++i) {
        NX_ARR_SET_EXPR(nx_i32, arr, i, i * i);
    }

    const nx_i32 val = 42;
    nx_arr_set(arr, 0, &val);

    const nx_cspan s = nx_arr_to_cspan(arr);

    nx_fprintln_cspan(stdout, s, nx_fprint_i32);

    nx_arr_drop(arr);

    return EXIT_SUCCESS;
}