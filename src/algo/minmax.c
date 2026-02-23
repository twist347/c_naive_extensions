#include "nx/algo/minmax.h"

#include "nx/core/assert.h"

nx_isize nx_min_element(nx_cspan s, nx_cmp_fn cmp) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(cmp);

    if (s.len == 0) {
        return -1;
    }

    nx_usize best = 0;
    const void *best_p = nx_cspan_get_c(s, 0);

    for (nx_usize i = 1; i < s.len; ++i) {
        const void *cur = nx_cspan_get_c(s, i);
        if (cmp(cur, best_p) < 0) {
            best = i;
            best_p = cur;
        }
    }

    return (nx_isize) best;
}

nx_isize nx_max_element(nx_cspan s, nx_cmp_fn cmp) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(cmp);

    if (s.len == 0) {
        return -1;
    }

    nx_usize best = 0;
    const void *best_p = nx_cspan_get_c(s, 0);

    for (nx_usize i = 1; i < s.len; ++i) {
        const void *cur = nx_cspan_get_c(s, i);
        if (cmp(cur, best_p) > 0) {
            best = i;
            best_p = cur;
        }
    }

    return (nx_isize) best;
}

nx_minmax nx_minmax_element(nx_cspan s, nx_cmp_fn cmp) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(cmp);

    if (s.len == 0) {
        return (nx_minmax){.min = -1, .max = -1};
    }

    nx_usize min_idx = 0;
    nx_usize max_idx = 0;
    const void *min_p = nx_cspan_get_c(s, 0);
    const void *max_p = nx_cspan_get_c(s, 0);

    for (nx_usize i = 1; i < s.len; ++i) {
        const void *cur = nx_cspan_get_c(s, i);

        if (cmp(cur, min_p) < 0) {
            min_idx = i;
            min_p = cur;
        }

        if (cmp(cur, max_p) > 0) {
            max_idx = i;
            max_p = cur;
        }
    }

    return (nx_minmax){
        .min = (nx_isize) min_idx,
        .max = (nx_isize) max_idx
    };
}
