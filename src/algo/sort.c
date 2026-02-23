#include "nx/algo/sort.h"

#include <stdlib.h>

#include "nx/core/assert.h"
#include "nx/core/panic.h"
#include "nx/core/util.h"

void nx_sort(nx_span s, nx_cmp_fn cmp) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(cmp);

    if (s.len < 2) {
        return;
    }

    qsort(s.data, s.len, s.tsz, cmp);
}

void nx_sort_ctx(nx_span s, nx_cmp_fn cmp, void *ctx) {
    NX_UNUSED(s);
    NX_UNUSED(cmp);
    NX_UNUSED(ctx);
    NX_UNIMPLEMENTED();
}

void nx_sort_stable(nx_span s, nx_cmp_fn cmp) {
    NX_UNUSED(s);
    NX_UNUSED(cmp);
    NX_UNIMPLEMENTED();
}

void nx_sort_stable_ctx(nx_span s, nx_cmp_fn cmp, void *ctx) {
    NX_UNUSED(s);
    NX_UNUSED(cmp);
    NX_UNUSED(ctx);
    NX_UNIMPLEMENTED();
}

void nx_partial_sort(nx_span s, nx_usize k, nx_cmp_fn cmp) {
    NX_UNUSED(s);
    NX_UNUSED(k);
    NX_UNUSED(cmp);
    NX_UNIMPLEMENTED();
}

void nx_nth_element(nx_span s, nx_usize nth, nx_cmp_fn cmp) {
    NX_UNUSED(s);
    NX_UNUSED(nth);
    NX_UNUSED(cmp);
    NX_UNIMPLEMENTED();
}

nx_bool nx_is_sorted_asc(nx_cspan s, nx_cmp_fn cmp) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(cmp);

    if (s.len < 2) {
        return true;
    }

    for (nx_usize i = 1; i < s.len; ++i) {
        const void *prev = nx_cspan_get_c(s, i - 1);
        const void *cur = nx_cspan_get_c(s, i);
        if (cmp(prev, cur) > 0) {
            return false;
        }
    }
    return true;
}

nx_bool nx_is_sorted_desc(nx_cspan s, nx_cmp_fn cmp) {
    NX_UNUSED(s);
    NX_UNUSED(cmp);
    NX_UNIMPLEMENTED();
}
