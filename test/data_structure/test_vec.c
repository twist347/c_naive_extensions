#include "criterion/criterion.h"

#include "nx/core/print.h"

#include "nx/data_structure/vec.h"

Test(A, B) {
    nx_vec *vec = nx_null;

    nx_status st = NX_VEC_NEW(&vec, nx_i32, 0);
    cr_assert_eq(st, NX_STATUS_OK);
    cr_assert_eq(nx_vec_len(vec), 0);
    cr_assert_eq(nx_vec_cap(vec), 0);

    const nx_i32 n = 10;

    for (nx_i32 i = 0; i < n; ++i) {
        st = nx_vec_push(vec, &i);
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
