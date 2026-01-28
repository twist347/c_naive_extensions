#include "nx/data_structure/arr.h"

#include "nx/core/print.h"
#include "nx/core/span.h"

int main(void) {
    nx_arr *arr = nx_null;

    const nx_status st = NX_ARR_MAKE(&arr, nx_i32, 5);
    if (st != NX_STATUS_OK) {
        // ...
    }

    const nx_usize len = nx_arr_len(arr);

    for (nx_usize i = 0; i < len; ++i) {
        NX_ARR_SET_EXPR(nx_i32, arr, i, i * i);
    }

    const nx_cspan s = nx_arr_to_cspan(arr);

    nx_fprint_cspan(stdout, s, nx_fprint_i32);

    nx_arr_drop(arr);
}