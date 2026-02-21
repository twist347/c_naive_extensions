#include "nx/algo/sort.h"

#include <stdlib.h>

#include "nx/core/assert.h"

void nx_sort(nx_span s, nx_cmp cmp) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(cmp);

    if (s.len < 2) {
        return;
    }

    qsort(s.data, s.len, s.tsz, cmp);
}

nx_bool nx_is_sorted(nx_cspan s, nx_cmp cmp) {
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
