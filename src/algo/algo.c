#include "nx/algo/algo.h"

#include <stdlib.h>
#include <string.h>

#include "nx/core/assert.h"
#include "nx/core/panic.h"

/* ---------- sort/order ---------- */

void nx_sort(nx_span s, nx_cmp cmp) {
    NX_ANY_SPAN_ASSERT(s);
    NX_ASSERT(cmp);

    if (s.len < 2) {
        return;
    }

    qsort(s.data, s.len, s.elem_size, cmp);
}

bool nx_is_sorted(nx_cspan s, nx_cmp cmp) {
    NX_ANY_SPAN_ASSERT(s);
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

/* ---------- search ---------- */

nx_isize nx_find(nx_cspan s, const void *key, nx_cmp cmp) {
    NX_ANY_SPAN_ASSERT(s);
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
    NX_ANY_SPAN_ASSERT(s);
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
    NX_ANY_SPAN_ASSERT(s);
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
    NX_ANY_SPAN_ASSERT(s);
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
    NX_ANY_SPAN_ASSERT(s);
    NX_ASSERT(key);
    NX_ASSERT(cmp);

    const nx_isize pos = nx_lower_bound(s, key, cmp);
    if ((nx_usize) pos >= s.len) {
        return -1;
    }

    const void *p = nx_cspan_get_c(s, pos);
    return cmp(p, key) == 0 ? pos : -1;
}

/* ---------- min/max ---------- */

nx_isize nx_min_element(nx_cspan s, nx_cmp cmp) {
    NX_ANY_SPAN_ASSERT(s);
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

    return (ptrdiff_t) best;
}

nx_isize nx_max_element(nx_cspan s, nx_cmp cmp) {
    NX_ANY_SPAN_ASSERT(s);
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

    return (ptrdiff_t) best;
}

nx_minmax nx_minmax_element(nx_cspan s, nx_cmp cmp) {
    NX_UNIMPLEMENTED();
}

/* ---------- comparison ---------- */

nx_bool nx_equal(nx_cspan a, nx_cspan b, nx_cmp cmp) {
    NX_ANY_SPAN_ASSERT(a);
    NX_ANY_SPAN_ASSERT(b);
    NX_ASSERT(a.elem_size == b.elem_size);
    NX_ASSERT(cmp);

    if (a.len != b.len) {
        return false;
    }

    for (nx_usize i = 0; i < a.len; ++i) {
        const void *ea = nx_cspan_get_c(a, i);
        const void *eb = nx_cspan_get_c(b, i);
        if (cmp(ea, eb) != 0) {
            return false;
        }
    }
    return true;
}

nx_bool nx_not_equal(nx_cspan a, nx_cspan b, nx_cmp cmp) {
    return !nx_equal(a, b, cmp);
}

void nx_fill(nx_span s, const void *elem) {
    NX_ANY_SPAN_ASSERT(s);
    NX_ASSERT(elem);

    if (s.len == 0) {
        return;
    }

    if (s.elem_size == 1) {
        memset(s.data, *(const unsigned char *) elem, s.len);
        return;
    }

    for (nx_usize i = 0; i < s.len; ++i) {
        memmove(nx_span_get(s, i), elem, s.elem_size);
    }
}

void nx_reverse(nx_span s) {
    NX_UNIMPLEMENTED();
}

void nx_swap_elements(nx_span s, nx_usize i, nx_usize j) {
    NX_UNIMPLEMENTED();
}
