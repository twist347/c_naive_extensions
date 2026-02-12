#include "nx/algo/algo_span.h"

#include <stdlib.h>
#include <string.h>

#include "nx/core/assert.h"
#include "nx/core/panic.h"

/* ========== sort/order ========== */

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

/* ========== search ========== */

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

/* ========== min/max ========== */

nx_isize nx_min_element(nx_cspan s, nx_cmp cmp) {
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

    return (ptrdiff_t) best;
}

nx_isize nx_max_element(nx_cspan s, nx_cmp cmp) {
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

    return (ptrdiff_t) best;
}

nx_minmax nx_minmax_element(nx_cspan s, nx_cmp cmp) {
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

/* ========== comparison ========== */

nx_bool nx_equal(nx_cspan a, nx_cspan b, nx_cmp cmp) {
    NX_SPAN_ANY_ASSERT(a);
    NX_SPAN_ANY_ASSERT(b);
    NX_ASSERT(a.tsz == b.tsz);
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

    // Reverse [0, mid)
    const nx_span left = nx_span_sub(s, 0, mid);
    nx_reverse(left);

    // Reverse [mid, len)
    const nx_span right = nx_span_sub(s, mid, s.len - mid);
    nx_reverse(right);

    // Reverse entire range
    nx_reverse(s);
}
