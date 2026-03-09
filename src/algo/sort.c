#include "nx/algo/sort.h"

#include <stdlib.h>

#include "nx/core/assert.h"
#include "nx/core/util.h"

// internal decls

static nx_usize median3(nx_CSpan s, nx_usize a, nx_usize b, nx_usize c, nx_cmp_fn cmp);
static nx_usize partition_lomuto(nx_Span s, nx_usize left, nx_usize right, nx_usize pivot_idx, nx_cmp_fn cmp);

// public api

void nx_sort(nx_Span s, nx_cmp_fn cmp) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(cmp);

    if (s.len < 2) {
        return;
    }

    qsort(s.data, s.len, s.tsz, cmp);
}

void nx_sort_stable(nx_Span s, nx_cmp_fn cmp) {
    NX_UNUSED(s);
    NX_UNUSED(cmp);
    NX_UNIMPLEMENTED();
}

void nx_partial_sort(nx_Span s, nx_usize k, nx_cmp_fn cmp) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(cmp);
    NX_ASSERT(k <= s.len);

    if (k <= 1 || s.len < 2) {
        return;
    }

    if (k >= s.len) {
        nx_sort(s, cmp);
        return;
    }

    // place element that would be at position k in sorted order
    nx_nth_element(s, k, cmp);

    // now the first k elements are the k smallest (order unspecified) -> sort them
    nx_sort(nx_span_sub(s, 0, k), cmp);
}

void nx_nth_element(nx_Span s, nx_usize nth, nx_cmp_fn cmp) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(cmp);
    NX_ASSERT(nth < s.len);

    if (s.len < 2) {
        return;
    }

    nx_usize left = 0;
    nx_usize right = s.len - 1;

    while (left < right) {
        const nx_usize mid = left + (right - left) / 2;
        const nx_usize pivot_index = median3(nx_cspan_from_span(s), left, mid, right, cmp);

        const nx_usize p = partition_lomuto(s, left, right, pivot_index, cmp);

        if (nth == p) {
            return;
        }
        if (nth < p) {
            // p is > 0 here because nth < p and nth is usize
            right = p - 1;
        } else {
            left = p + 1;
        }
    }
}

nx_bool nx_is_sorted(nx_CSpan s, nx_cmp_fn cmp) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(cmp);

    if (s.len < 2) {
        return true;
    }

    for (nx_usize i = 1; i < s.len; ++i) {
        const void *prev = nx_cspan_get_c(s, i - 1);
        const void *cur  = nx_cspan_get_c(s, i);

        // "sorted" means: prev <= cur according to cmp
        if (cmp(prev, cur) > 0) {
            return false;
        }
    }
    return true;
}

// internal defs

static nx_usize median3(nx_CSpan s, nx_usize a, nx_usize b, nx_usize c, nx_cmp_fn cmp) {
    const void *a_ptr = nx_cspan_get_c(s, a);
    const void *b_ptr = nx_cspan_get_c(s, b);
    const void *c_ptr = nx_cspan_get_c(s, c);

    const int ab = cmp(a_ptr, b_ptr);
    const int ac = cmp(a_ptr, c_ptr);
    const int bc = cmp(b_ptr, c_ptr);

    // A between B and C
    if ((ab <= 0 && ac >= 0) || (ab >= 0 && ac <= 0)) return a;
    // B between A and C
    if ((ab >= 0 && bc <= 0) || (ab <= 0 && bc >= 0)) return b;
    // else C is median
    return c;
}

static nx_usize partition_lomuto(nx_Span s, nx_usize left, nx_usize right, nx_usize pivot_idx, nx_cmp_fn cmp) {
    NX_ASSERT(left <= pivot_idx && pivot_idx <= right);

    nx_span_swap(s, pivot_idx, right);
    const void *pivot = nx_span_get_c(s, right);

    nx_usize i = left;
    for (nx_usize j = left; j < right; ++j) {
        const void *x = nx_span_get_c(s, j);
        if (cmp(x, pivot) < 0) {
            nx_span_swap(s, i, j);
            ++i;
        }
    }

    nx_span_swap(s, i, right);
    return i;
}
