#include "criterion/criterion.h"

#include "nx/algo/algo.h"
#include "nx/core/print.h"
#include "nx/core/type.h"

Test(A, B) {
    nx_i32 arr[] = {2, 1, 3, 5, 2, 4};
    const nx_span s = NX_SPAN_FROM_PTR(arr, 5);
    const nx_cspan cs = NX_CSPAN_FROM_PTR(arr, 5);

    nx_fprintln_cspan(stdout, cs, nx_fprint_i32);

    nx_sort(s, nx_cmp_i32);
    nx_fprintln_cspan(stdout, cs, nx_fprint_i32);

    cr_assert(nx_is_sorted(cs, nx_cmp_i32));

    const nx_i32 x = 2;
    const nx_usize c = nx_count(cs, &x, nx_cmp_i32);
    cr_assert_eq(c, 2);
}
