#include "nx/algo/transform.h"

#include <string.h>

#include "nx/core/panic.h"

void nx_fill(nx_span s, const void *elem) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(elem);

    if (s.len == 0) {
        return;
    }

    if (s.tsz == 1) {
        memset(s.data, *(const nx_byte *) elem, s.len);
        return;
    }

    for (nx_usize i = 0; i < s.len; ++i) {
        memmove(nx_span_get(s, i), elem, s.tsz);
    }
}

void nx_reverse(nx_span s) {
    NX_SPAN_ANY_ASSERT(s);

    if (s.len < 2) {
        return;
    }

    nx_usize left = 0;
    nx_usize right = s.len - 1;

    while (left < right) {
        nx_swap_elements(s, left, right);
        ++left;
        --right;
    }
}

void nx_swap_elements(nx_span s, nx_usize i, nx_usize j) {
    NX_UNIMPLEMENTED();
}

void nx_rotate(nx_span s, nx_usize mid) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(mid <= s.len);

    if (mid == 0 || mid == s.len) {
        return;
    }

    // reverse [0, mid)
    const nx_span left = nx_span_sub(s, 0, mid);
    nx_reverse(left);

    // reverse [mid, len)
    const nx_span right = nx_span_sub(s, mid, s.len - mid);
    nx_reverse(right);

    // reverse entire range
    nx_reverse(s);
}

void nx_transform(nx_span dst, nx_cspan src, nx_transform_fn fn) {
    NX_SPAN_ANY_ASSERT(dst);
    NX_SPAN_ANY_ASSERT(src);
    NX_ASSERT(dst.len == src.len);
    NX_ASSERT(dst.tsz == src.tsz);
    NX_ASSERT(fn);

    for (nx_usize i = 0; i < src.len; ++i) {
        fn(nx_span_get(dst, i), nx_cspan_get_c(src, i));
    }
}

void nx_apply(nx_span s, nx_apply_fn fn) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(fn);

    for (nx_usize i = 0; i < s.len; ++i) {
        fn(nx_span_get(s, i));
    }
}

