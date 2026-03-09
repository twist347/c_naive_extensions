#include "nx/algo/transform.h"

#include <string.h>

#include "nx/core/assert.h"

void nx_fill(nx_Span s, const void *elem) {
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

void nx_reverse(nx_Span s) {
    NX_SPAN_ANY_ASSERT(s);

    if (s.len < 2) {
        return;
    }

    nx_usize left = 0;
    nx_usize right = s.len - 1;

    while (left < right) {
        nx_span_swap(s, left, right);
        ++left;
        --right;
    }
}

void nx_rotate(nx_Span s, nx_usize mid) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(mid <= s.len);

    if (mid == 0 || mid == s.len) {
        return;
    }

    // reverse [0, mid)
    const nx_Span left = nx_span_sub(s, 0, mid);
    nx_reverse(left);

    // reverse [mid, len)
    const nx_Span right = nx_span_sub(s, mid, s.len - mid);
    nx_reverse(right);

    // reverse entire range
    nx_reverse(s);
}

void nx_transform(nx_Span dst, nx_CSpan src, nx_transform_fn fn) {
    NX_SPAN_ANY_ASSERT(dst);
    NX_SPAN_ANY_ASSERT(src);
    NX_ASSERT(dst.len == src.len);
    NX_ASSERT(dst.tsz == src.tsz);
    NX_ASSERT(fn);

    for (nx_usize i = 0; i < src.len; ++i) {
        fn(nx_span_get(dst, i), nx_cspan_get_c(src, i));
    }
}

/* ========== copy ========== */

void nx_copy(nx_Span dst, nx_CSpan src) {
    NX_SPAN_ANY_ASSERT(dst);
    NX_SPAN_ANY_ASSERT(src);
    NX_ASSERT(dst.len == src.len);
    NX_ASSERT(dst.tsz == src.tsz);

    if (dst.len == 0) {
        return;
    }

    const nx_usize bytes = dst.len * dst.tsz; // no overflow checking policy
    memmove(dst.data, src.data, bytes);
}

nx_usize nx_copy_if(nx_Span dst, nx_CSpan src, nx_predicate_fn pred) {
    NX_SPAN_ANY_ASSERT(dst);
    NX_SPAN_ANY_ASSERT(src);
    NX_ASSERT(dst.tsz == src.tsz);
    NX_ASSERT(dst.len >= src.len);
    NX_ASSERT(pred);

    nx_usize out = 0;
    for (nx_usize i = 0; i < src.len; ++i) {
        const void *e = nx_cspan_get_c(src, i);
        if (pred(e)) {
            memmove(nx_span_get(dst, out), e, src.tsz);
            ++out;
        }
    }
    return out;
}

/* ========== replace ========== */

nx_usize nx_replace(nx_Span s, const void *old_val, const void *new_val, nx_cmp_fn cmp) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(old_val);
    NX_ASSERT(new_val);
    NX_ASSERT(cmp);

    nx_usize count = 0;
    for (nx_usize i = 0; i < s.len; ++i) {
        void *e = nx_span_get(s, i);
        if (cmp(e, old_val) == 0) {
            memmove(e, new_val, s.tsz);
            ++count;
        }
    }
    return count;
}

nx_usize nx_replace_if(nx_Span s, nx_predicate_fn pred, const void *new_val) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(pred);
    NX_ASSERT(new_val);

    nx_usize count = 0;
    for (nx_usize i = 0; i < s.len; ++i) {
        void *e = nx_span_get(s, i);
        if (pred(e)) {
            memmove(e, new_val, s.tsz);
            ++count;
        }
    }
    return count;
}

/* ========== generate ========== */

void nx_generate(nx_Span s, nx_generate_fn gen) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(gen);

    for (nx_usize i = 0; i < s.len; ++i) {
        gen(nx_span_get(s, i));
    }
}

/* ========== for_each ========== */

void nx_for_each(nx_Span s, nx_for_each_fn fn) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(fn);

    for (nx_usize i = 0; i < s.len; ++i) {
        fn(nx_span_get(s, i));
    }
}

void nx_for_each_c(nx_CSpan s, nx_for_each_c_fn fn) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(fn);

    for (nx_usize i = 0; i < s.len; ++i) {
        fn(nx_cspan_get_c(s, i));
    }
}

/* ========== ? ========== */

nx_usize nx_remove(nx_Span s, const void *key, nx_cmp_fn cmp) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(key);
    NX_ASSERT(cmp);

    if (s.len == 0) {
        return 0;
    }

    nx_usize out = 0;
    for (nx_usize i = 0; i < s.len; ++i) {
        const void *elem = nx_span_get(s, i);
        if (cmp(elem, key) != 0) {
            if (out != i) {
                memmove(nx_span_get(s, out), elem, s.tsz);
            }
            ++out;
        }
    }
    return out;
}

nx_usize nx_remove_if(nx_Span s, nx_predicate_fn pred) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(pred);

    if (s.len == 0) {
        return 0;
    }

    nx_usize out = 0;
    for (nx_usize i = 0; i < s.len; ++i) {
        const void *elem = nx_span_get(s, i);
        if (!pred(elem)) {
            if (out != i) {
                memmove(nx_span_get(s, out), elem, s.tsz);
            }
            ++out;
        }
    }
    return out;
}

nx_usize nx_unique(nx_Span s, nx_cmp_fn cmp) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(cmp);

    if (s.len < 2) {
        return s.len;
    }

    nx_usize out = 1; // keep first
    for (nx_usize i = 1; i < s.len; ++i) {
        const void *last = nx_span_get_c(s, out - 1);
        const void *cur = nx_span_get_c(s, i);
        if (cmp(last, cur) != 0) {
            if (out != i) {
                memmove(nx_span_get(s, out), nx_span_get(s, i), s.tsz);
            }
            ++out;
        }
    }
    return out;
}
