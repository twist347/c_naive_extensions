#include "nx/algo/sort.h"

#include <stdlib.h>
#include <string.h>

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
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(cmp);

    if (s.len < 2) {
        return;
    }

    const nx_usize tsz = s.tsz;
    const nx_usize len = s.len;
    const nx_usize bytes = len * tsz;

    nx_byte *buf = malloc(bytes);
    NX_VERIFY(buf); // merge sort needs O(n) temp space

    nx_byte *src = s.data;
    nx_byte *dst = buf;

    // bottom-up merge sort: merge runs of width 1, 2, 4, ...
    for (nx_usize width = 1; width < len; width *= 2) {
        for (nx_usize i = 0; i < len; i += 2 * width) {
            // merge [i, i+width) and [i+width, i+2*width) into dst
            nx_usize left = i;
            nx_usize right = i + width;
            const nx_usize left_end = right < len ? right : len;
            const nx_usize right_end = i + 2 * width < len ? i + 2 * width : len;
            nx_usize out = i;

            while (left < left_end && right < right_end) {
                const void *l = src + left * tsz;
                const void *r = src + right * tsz;
                if (cmp(l, r) <= 0) {
                    memcpy(dst + out * tsz, l, tsz);
                    ++left;
                } else {
                    memcpy(dst + out * tsz, r, tsz);
                    ++right;
                }
                ++out;
            }

            // copy remaining left
            if (left < left_end) {
                memcpy(dst + out * tsz, src + left * tsz, (left_end - left) * tsz);
            }

            // copy remaining right
            if (right < right_end) {
                memcpy(dst + out * tsz, src + right * tsz, (right_end - right) * tsz);
            }
        }

        // swap src and dst for next pass
        nx_byte *tmp = src;
        src = dst;
        dst = tmp;
    }

    // if result ended up in buf, copy back to original
    if (src != (nx_byte *) s.data) {
        memcpy(s.data, src, bytes);
    }

    free(buf);
}

void nx_partial_sort(nx_Span s, nx_usize k, nx_cmp_fn cmp) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(cmp);
    NX_ASSERT(k <= s.len);

    if (k == 0 || s.len < 2) {
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
        const void *cur = nx_cspan_get_c(s, i);

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
