#include "criterion/criterion.h"

#include <stdio.h>

#include "nx/core/span.h"
#include "nx/core/print.h"
#include "nx/core/type.h"

Test(A, B) {
    nx_i32 arr[] = {1, 2, 3, 4, 5};

    const nx_span s = NX_SPAN_FROM_PTR(arr, 5);

    const nx_i32 x = 100;
    nx_span_set(s, 0, &x);

    nx_fprintln_span(stdout, s, nx_fprint_i32);
}
