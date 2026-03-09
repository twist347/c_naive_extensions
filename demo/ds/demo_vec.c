#include "nx/ds/vec.h"

#include <stdio.h>
#include <stdlib.h>

#include "nx/ds/span.h"
#include "nx/io/print.h"

int main(void) {
    nx_VecRes res = NX_VEC_NEW(int);
    if (!NX_RES_IS_OK(res)) {
        nx_fprintf(stderr, nx_status_to_str(NX_RES_ERR(res)));
        return EXIT_FAILURE;
    }

    nx_Vec *vec = NX_RES_UNWRAP(res);

    for (nx_i32 i = 0; i < 10; ++i) {
        NX_VEC_PUSH_EXPR(nx_i32, vec, i * i);
    }

    const nx_CSpan s = nx_vec_to_cspan(vec);

    nx_cspan_fprintln(s, stdout, nx_fprint_i32);

    nx_vec_drop(vec);
}
