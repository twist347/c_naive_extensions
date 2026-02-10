#include "nx/str/str_view.h"

#include <ctype.h>
#include <string.h>

#include "nx/core/assert.h"

/* ---------- assert ---------- */

void nx_str_view_assert_(nx_str_view sv) {
    NX_ASSERT(sv.len == 0 || sv.data != nx_null);
}

/* ========== lifetime ========== */

nx_str_view nx_str_view_new(const nx_char *data, nx_usize len) {
    const nx_str_view sv = {.data = data, .len = len};

    NX_STR_VIEW_ASSERT(sv);

    return sv;
}

nx_str_view nx_str_view_from_cstr(const nx_char *cstr) {
    NX_ASSERT(cstr);

    return nx_str_view_new(cstr, strlen(cstr));
}

/* ========== comparisons ========== */

nx_bool nx_str_view_eq(nx_str_view a, nx_str_view b) {
    NX_STR_VIEW_ASSERT(a);
    NX_STR_VIEW_ASSERT(b);

    if (a.len != b.len) {
        return false;
    }

    if (a.len == 0) {
        return true;
    }

    return memcmp(a.data, b.data, a.len) == 0;
}

nx_bool nx_str_view_not_eq(nx_str_view a, nx_str_view b) {
    return !nx_str_view_eq(a, b);
}

nx_i32 nx_str_view_cmp(nx_str_view a, nx_str_view b) {
    NX_STR_VIEW_ASSERT(a);
    NX_STR_VIEW_ASSERT(b);

    const nx_usize n = a.len < b.len ? a.len : b.len;

    if (n != 0) {
        const int r = memcmp(a.data, b.data, n);
        if (r < 0) {
            return -1;
        }
        if (r > 0) {
            return 1;
        }
    }
    if (a.len < b.len) {
        return -1;
    }
    if (a.len > b.len) {
        return 1;
    }

    return 0;
}

/* ========== prefix/suffix ========== */

nx_bool nx_sv_starts_with(nx_str_view s, nx_str_view prefix) {
    NX_STR_VIEW_ASSERT(s);
    NX_STR_VIEW_ASSERT(prefix);

    if (prefix.len > s.len) {
        return false;
    }
    if (prefix.len == 0) {
        return true;
    }

    return memcmp(s.data, prefix.data, prefix.len) == 0;
}

nx_bool nx_sv_ends_with(nx_str_view s, nx_str_view suffix) {
    NX_STR_VIEW_ASSERT(s);
    NX_STR_VIEW_ASSERT(suffix);

    if (suffix.len > s.len) {
        return false;
    }
    if (suffix.len == 0) {
        return true;
    }

    const nx_usize offset = s.len - suffix.len;
    return memcmp(s.data + offset, suffix.data, suffix.len) == 0;
}

/* ========== slice ========== */

nx_str_view nx_str_view_sub(nx_str_view s, nx_usize pos, nx_usize len) {
    NX_STR_VIEW_ASSERT(s);
    NX_ASSERT(pos <= s.len);

    const nx_usize available = s.len - pos;
    if (len > available) {
        len = available;
    }

    return nx_str_view_new(s.data + pos, len);
}

/* ========== search ========== */

nx_isize nx_str_view_find_sub(nx_str_view s, nx_str_view needle) {
    NX_STR_VIEW_ASSERT(s);
    NX_STR_VIEW_ASSERT(needle);

    if (needle.len == 0) {
        return 0;
    }
    if (needle.len > s.len) {
        return -1;
    }

    /* naive search */
    const nx_usize last = s.len - needle.len;
    for (nx_usize i = 0; i <= last; ++i) {
        if (s.data[i] == needle.data[0]) {
            if (memcmp(s.data + i, needle.data, needle.len) == 0) {
                return (nx_isize) i;
            }
        }
    }
    return -1;
}

nx_isize nx_str_view_find_char(nx_str_view s, nx_char ch) {
    NX_STR_VIEW_ASSERT(s);

    for (nx_usize i = 0; i < s.len; ++i) {
        if (s.data[i] == ch) {
            return (nx_isize) i;
        }
    }
    return -1;
}

/* ========== trim ========== */

nx_str_view nx_str_view_trim_start(nx_str_view s) {
    NX_STR_VIEW_ASSERT(s);

    nx_usize i = 0;
    while (i < s.len && isspace(s.data[i])) {
        ++i;
    }

    return nx_str_view_new(s.data + i, s.len - i);
}

nx_str_view nx_str_view_trim_end(nx_str_view s) {
    NX_STR_VIEW_ASSERT(s);

    nx_usize i = s.len;
    while (i > 0 && isspace(s.data[i - 1])) {
        --i;
    }

    return nx_str_view_new(s.data, i);
}

nx_str_view nx_str_view_trim(nx_str_view s) {
    NX_STR_VIEW_ASSERT(s);

    return nx_str_view_trim_end(nx_str_view_trim_start(s));
}

