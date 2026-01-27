#include "criterion/criterion.h"

#include <stdio.h>

#include "nx/core/span.h"
#include "nx/core/print.h"

Test(A, B) {
    int arr[] = {1, 2, 3, 4, 5};

    const nx_span s = NX_SPAN_FROM_PTR(arr, 5);

    const int x = 100;
    nx_span_set(s, 0, &x);

    nx_fprint_span(stdout, s, nx_fprint_i);

    printf("\n");
}
