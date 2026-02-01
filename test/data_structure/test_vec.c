#include "criterion/criterion.h"

#include "nx/core/print.h"

#include "nx/data_structure/vec.h"

Test(A, B) {
    nx_vec_res res = NX_VEC_NEW(int);
    if (!NX_RES_IS_OK(res)) {
        nx_status_fprint(stderr, NX_RES_GET_ERR(res));
        return;
    }

    nx_vec *vec = NX_RES_GET_VAL(res);
    cr_assert_eq(nx_vec_len(vec), 0);
    cr_assert_eq(nx_vec_cap(vec), 0);

    const nx_i32 n = 10;

    for (nx_i32 i = 0; i < n; ++i) {
        const nx_status st = nx_vec_push(vec, &i);
        cr_assert_eq(st, NX_STATUS_OK);
    }

    for (nx_i32 i = 0; i < n; ++i) {
        cr_assert_eq(i, *NX_VEC_GET_AS_C(nx_i32, vec, i));
    }

    cr_assert_eq(nx_vec_len(vec), n);

    const nx_usize cap = nx_vec_cap(vec);
    nx_fprint_usize(stdout, &cap);

    nx_vec_drop(vec);
}
