#include "criterion/criterion.h"

#include "nx/algo/algo.h"
#include "nx/core/print.h"

Test(A, B) {
    int arr[] = {2, 1, 3, 5, 2, 4};
    const nx_span s = NX_SPAN_FROM_PTR(arr, 5);
    const nx_cspan cs = NX_CSPAN_FROM_PTR(arr, 5);

    nx_fprint_cspan(stdout, cs, nx_fprint_i);

    nx_sort(s, nx_cmp_i);
    nx_fprint_cspan(stdout, cs, nx_fprint_i);

    cr_assert(nx_is_sorted(cs, nx_cmp_i));

    const int x = 2;
    const size_t c = nx_count(cs, &x, nx_cmp_i);
    cr_assert_eq(c, 2);
}
