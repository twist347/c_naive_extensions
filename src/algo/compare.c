#include "nx/algo/compare.h"

#include "nx/core/assert.h"

nx_bool nx_equal(nx_cspan a, nx_cspan b, nx_cmp_fn cmp) {
    NX_SPAN_ANY_ASSERT(a);
    NX_SPAN_ANY_ASSERT(b);
    NX_ASSERT(a.tsz == b.tsz);
    NX_ASSERT(cmp);

    if (a.len != b.len) {
        return false;
    }

    for (nx_usize i = 0; i < a.len; ++i) {
        const void *ea = nx_cspan_get_c(a, i);
        const void *eb = nx_cspan_get_c(b, i);
        if (cmp(ea, eb) != 0) {
            return false;
        }
    }
    return true;
}

nx_bool nx_not_equal(nx_cspan a, nx_cspan b, nx_cmp_fn cmp) {
    return !nx_equal(a, b, cmp);
}

