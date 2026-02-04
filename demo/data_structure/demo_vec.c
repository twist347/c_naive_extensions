#include "nx/data_structure/vec.h"

#include <stdio.h>
#include <stdlib.h>

#include "nx/core/span.h"
#include "nx/core/print.h"

int main(void) {
    nx_vec_res res = NX_VEC_NEW(int);
    if (!NX_RES_IS_OK(res)) {
        nx_status_fprintln(stderr, NX_RES_ERR(res));
        return EXIT_FAILURE;
    }

    nx_vec *vec = NX_RES_VAL(res);

    for (nx_i32 i = 0; i < 10; ++i) {
        NX_VEC_PUSH_EXPR(nx_i32, vec, i * i);
    }

    const nx_cspan s = nx_vec_to_cspan(vec);

    nx_fprintln_cspan(stdout, s, nx_fprint_i32);

    nx_vec_drop(vec);
}
