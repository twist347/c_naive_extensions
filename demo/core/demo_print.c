#include "nx/core/print.h"

#include "nx/ds/span.h"

int main(void) {
    const nx_i32 x = 5;

    nx_printf("" NX_FMT_I32 "\n", x);

    const nx_i32 arr[] = {1, 2, 3, 4, 5};

    nx_cspan cs = NX_CSPAN_FROM_PTR(arr, 5);

    nx_println_cspan(cs, nx_fprint_i32);

    nx_dbg(2 + 3);
}
