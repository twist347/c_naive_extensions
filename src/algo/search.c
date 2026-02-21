#include "nx/algo/search.h"

#include "nx/core/assert.h"

nx_isize nx_find(nx_cspan s, const void *key, nx_cmp cmp) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(key);
    NX_ASSERT(cmp);

    for (nx_usize i = 0; i < s.len; ++i) {
        const void *elem = nx_cspan_get_c(s, i);
        if (cmp(elem, key) == 0) {
            return (ptrdiff_t) i;
        }
    }
    return -1;
}

nx_bool nx_contains(nx_cspan s, const void *key, nx_cmp cmp) {
    return nx_find(s, key, cmp) != -1;
}

nx_usize nx_count(nx_cspan s, const void *key, nx_cmp cmp) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(key);
    NX_ASSERT(cmp);

    nx_usize count = 0;
    for (nx_usize i = 0; i < s.len; ++i) {
        const void *elem = nx_cspan_get_c(s, i);
        if (cmp(elem, key) == 0) {
            ++count;
        }
    }
    return count;
}

nx_isize nx_lower_bound(nx_cspan s, const void *key, nx_cmp cmp) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(key);
    NX_ASSERT(cmp);

    nx_usize lo = 0;
    nx_usize hi = s.len;

    while (lo < hi) {
        const nx_usize mid = lo + (hi - lo) / 2;
        const void *midp = nx_cspan_get_c(s, mid);

        if (cmp(midp, key) < 0) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }

    return (ptrdiff_t) lo;
}

nx_isize nx_upper_bound(nx_cspan s, const void *key, nx_cmp cmp) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(key);
    NX_ASSERT(cmp);

    nx_usize lo = 0;
    nx_usize hi = s.len;

    while (lo < hi) {
        const nx_usize mid = lo + (hi - lo) / 2;
        const void *midp = nx_cspan_get_c(s, mid);

        if (cmp(midp, key) <= 0) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }

    return (ptrdiff_t) lo;
}

nx_isize nx_bsearch(nx_cspan s, const void *key, nx_cmp cmp) {
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(key);
    NX_ASSERT(cmp);

    const nx_isize pos = nx_lower_bound(s, key, cmp);
    if ((nx_usize) pos >= s.len) {
        return -1;
    }

    const void *p = nx_cspan_get_c(s, pos);
    return cmp(p, key) == 0 ? pos : -1;
}
