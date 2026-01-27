#include "criterion/criterion.h"

#include "nx/core/print.h"

#include "nx/data_structure/vec.h"

Test(A, B) {
    nx_vec *vec = NULL;

    nx_status st = NX_VEC_MAKE(&vec, int, 0);
    cr_assert_eq(st, NX_STATUS_OK);
    cr_assert_eq(nx_vec_len(vec), 0);
    cr_assert_eq(nx_vec_cap(vec), 0);

    const int n = 10;

    for (int i = 0; i < n; ++i) {
        st = nx_vec_push(vec, &i);
        cr_assert_eq(st, NX_STATUS_OK);
    }

    for (int i = 0; i < n; ++i) {
        cr_assert_eq(i, *NX_VEC_GET_AS_C(int, vec, i));
    }

    cr_assert_eq(nx_vec_len(vec), n);
    printf("%zu\n", nx_vec_cap(vec));

    nx_vec_drop(vec);
}
