#include "nx/algo/algo.h"

#include <stdlib.h>
#include <string.h>

#include "nx/core/assert.h"
#include "nx/core/panic.h"

/* ---------- sort/order ---------- */

void nx_sort(nx_span s, nx_cmp cmp) {
    NX_SPAN_ASSERT(s);
    NX_ASSERT(cmp);

    qsort(s.data, s.len, s.elem_size, cmp);
}

bool nx_is_sorted(nx_cspan s, nx_cmp cmp) {
    NX_SPAN_ASSERT(s);
    NX_ASSERT(cmp);

    if (s.len < 2) {
        return true;
    }

    for (size_t i = 1; i < s.len; ++i) {
        const void *prev = nx_cspan_get(s, i - 1);
        const void *cur = nx_cspan_get(s, i);
        if (cmp(prev, cur) > 0) {
            return false;
        }
    }
    return true;
}

/* ---------- search ---------- */

ptrdiff_t nx_find(nx_cspan s, const void *key, nx_cmp cmp) {
    NX_SPAN_ASSERT(s);
    NX_ASSERT(key);
    NX_ASSERT(cmp);

    for (size_t i = 0; i < s.len; ++i) {
        const void *elem = nx_cspan_get(s, i);
        if (cmp(elem, key) == 0) {
            return (ptrdiff_t) i;
        }
    }
    return -1;
}

bool nx_contains(nx_cspan s, const void *key, nx_cmp cmp) {
    return nx_find(s, key, cmp) != -1;
}

size_t nx_count(nx_cspan s, const void *key, nx_cmp cmp) {
    NX_SPAN_ASSERT(s);
    NX_ASSERT(key);
    NX_ASSERT(cmp);

    size_t count = 0;
    for (size_t i = 0; i < s.len; ++i) {
        const void *elem = nx_cspan_get(s, i);
        if (cmp(elem, key) == 0) {
            ++count;
        }
    }
    return count;
}

ptrdiff_t nx_lower_bound(nx_cspan s, const void *key, nx_cmp cmp) {
    NX_SPAN_ASSERT(s);
    NX_ASSERT(key);
    NX_ASSERT(cmp);

    size_t lo = 0;
    size_t hi = s.len;

    while (lo < hi) {
        const size_t mid = lo + (hi - lo) / 2;
        const void *midp = nx_cspan_get(s, mid);

        if (cmp(midp, key) < 0) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }

    return (ptrdiff_t) lo;
}

ptrdiff_t nx_upper_bound(nx_cspan s, const void *key, nx_cmp cmp) {
    NX_SPAN_ASSERT(s);
    NX_ASSERT(key);
    NX_ASSERT(cmp);

    size_t lo = 0;
    size_t hi = s.len;

    while (lo < hi) {
        const size_t mid = lo + (hi - lo) / 2;
        const void *midp = nx_cspan_get(s, mid);

        if (cmp(midp, key) <= 0) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }

    return (ptrdiff_t) lo;
}

ptrdiff_t nx_bsearch(nx_cspan s, const void *key, nx_cmp cmp) {
    NX_SPAN_ASSERT(s);
    NX_ASSERT(key);
    NX_ASSERT(cmp);

    const ptrdiff_t pos = nx_lower_bound(s, key, cmp);
    if ((size_t) pos >= s.len) {
        return -1;
    }

    const void *p = nx_cspan_get(s, pos);
    return cmp(p, key) == 0 ? pos : -1;
}

/* ---------- min/max ---------- */

ptrdiff_t nx_min_element(nx_cspan s, nx_cmp cmp) {
    NX_UNIMPLEMENTED();
}
ptrdiff_t nx_max_element(nx_cspan s, nx_cmp cmp) {
    NX_UNIMPLEMENTED();
}

/* ---------- comparison ---------- */

bool nx_equal(nx_cspan a, nx_cspan b, nx_cmp cmp) {
    NX_SPAN_ASSERT(a);
    NX_SPAN_ASSERT(b);
    NX_ASSERT(a.elem_size == b.elem_size);
    NX_ASSERT(cmp);

    if (a.len != b.len) {
        return false;
    }

    for (size_t i = 0; i < a.len; ++i) {
        const void *ea = nx_cspan_get(a, i);
        const void *eb = nx_cspan_get(b, i);
        if (cmp(ea, eb) != 0) {
            return false;
        }
    }
    return true;
}

bool nx_not_equal(nx_cspan a, nx_cspan b, nx_cmp cmp) {
    return !nx_equal(a, b, cmp);
}

void nx_fill(nx_span s, const void *elem) {
    NX_SPAN_ASSERT(s);
    NX_ASSERT(elem);

    if (s.len == 0) {
        return;
    }

    if (s.elem_size == 1) {
        memset(s.data, *(const unsigned char *) elem, s.len);
        return;
    }

    for (size_t i = 0; i < s.len; ++i) {
        memmove(nx_span_get(s, i), elem, s.elem_size);
    }
}
