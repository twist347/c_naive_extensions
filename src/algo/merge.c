#include "nx/algo/merge.h"

#include <stdlib.h>
#include <string.h>

#include "nx/core/assert.h"

void nx_merge(nx_Span dst, nx_CSpan a, nx_CSpan b, nx_cmp_fn cmp) {
    NX_SPAN_ANY_ASSERT(dst);
    NX_SPAN_ANY_ASSERT(a);
    NX_SPAN_ANY_ASSERT(b);
    NX_ASSERT(dst.tsz == a.tsz);
    NX_ASSERT(dst.tsz == b.tsz);
    NX_ASSERT(dst.len == a.len + b.len);
    NX_ASSERT(cmp);

    const nx_usize tsz = dst.tsz;
    nx_usize i = 0;
    nx_usize j = 0;
    nx_usize out = 0;

    while (i < a.len && j < b.len) {
        const void *l = nx_cspan_get_c(a, i);
        const void *r = nx_cspan_get_c(b, j);

        if (cmp(l, r) <= 0) {
            memcpy(nx_span_get(dst, out), l, tsz);
            ++i;
        } else {
            memcpy(nx_span_get(dst, out), r, tsz);
            ++j;
        }
        ++out;
    }

    while (i < a.len) {
        memcpy(nx_span_get(dst, out), nx_cspan_get_c(a, i), tsz);
        ++i;
        ++out;
    }

    while (j < b.len) {
        memcpy(nx_span_get(dst, out), nx_cspan_get_c(b, j), tsz);
        ++j;
        ++out;
    }
}

void nx_inplace_merge(nx_Span s, nx_usize mid, nx_cmp_fn cmp) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(mid <= s.len);
    NX_ASSERT(cmp);

    if (mid == 0 || mid == s.len || s.len < 2) {
        return;
    }

    const nx_usize tsz = s.tsz;
    const nx_usize bytes = s.len * tsz;

    nx_byte *buf = malloc(bytes);
    NX_VERIFY(buf);

    const nx_CSpan left = nx_cspan_sub(nx_cspan_from_span(s), 0, mid);
    const nx_CSpan right = nx_cspan_sub(nx_cspan_from_span(s), mid, s.len - mid);

    const nx_Span dst = nx_span_new(buf, s.len, tsz);
    nx_merge(dst, left, right, cmp);

    memcpy(s.data, buf, bytes);
    free(buf);
}
