#include "nx/string/str_view.h"

#include <ctype.h>
#include <string.h>

#include "nx/core/assert.h"

/* ========== assert ========== */

void nx_str_view_assert_(nx_StrView sv) {
    NX_ASSERT(sv.len == 0 || sv.data != nx_null);
}

/* ========== lifetime ========== */

nx_StrView nx_str_view_new(const nx_char *data, nx_usize len) {
    const nx_StrView sv = {.data = data, .len = len};

    NX_STR_VIEW_ASSERT(sv);

    return sv;
}

nx_StrView nx_str_view_from_cstr(const nx_char *cstr) {
    NX_ASSERT(cstr);

    return nx_str_view_new(cstr, strlen(cstr));
}

/* ========== comparisons ========== */

nx_bool nx_str_view_eq(nx_StrView a, nx_StrView b) {
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

nx_bool nx_str_view_not_eq(nx_StrView a, nx_StrView b) {
    return !nx_str_view_eq(a, b);
}

nx_i32 nx_str_view_cmp(nx_StrView a, nx_StrView b) {
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

nx_bool nx_sv_starts_with(nx_StrView self, nx_StrView prefix) {
    NX_STR_VIEW_ASSERT(self);
    NX_STR_VIEW_ASSERT(prefix);

    if (prefix.len > self.len) {
        return false;
    }
    if (prefix.len == 0) {
        return true;
    }

    return memcmp(self.data, prefix.data, prefix.len) == 0;
}

nx_bool nx_sv_ends_with(nx_StrView self, nx_StrView suffix) {
    NX_STR_VIEW_ASSERT(self);
    NX_STR_VIEW_ASSERT(suffix);

    if (suffix.len > self.len) {
        return false;
    }
    if (suffix.len == 0) {
        return true;
    }

    const nx_usize offset = self.len - suffix.len;
    return memcmp(self.data + offset, suffix.data, suffix.len) == 0;
}

/* ========== slice ========== */

nx_StrView nx_str_view_sub(nx_StrView self, nx_usize pos, nx_usize len) {
    NX_STR_VIEW_ASSERT(self);
    NX_ASSERT(pos <= self.len);

    const nx_usize available = self.len - pos;
    if (len > available) {
        len = available;
    }

    return nx_str_view_new(self.data + pos, len);
}

/* ========== search ========== */

nx_isize nx_str_view_find_sub(nx_StrView self, nx_StrView needle) {
    NX_STR_VIEW_ASSERT(self);
    NX_STR_VIEW_ASSERT(needle);

    if (needle.len == 0) {
        return 0;
    }
    if (needle.len > self.len) {
        return -1;
    }

    /* naive search */
    const nx_usize last = self.len - needle.len;
    for (nx_usize i = 0; i <= last; ++i) {
        if (self.data[i] == needle.data[0]) {
            if (memcmp(self.data + i, needle.data, needle.len) == 0) {
                return (nx_isize) i;
            }
        }
    }
    return -1;
}

nx_isize nx_str_view_find_char(nx_StrView self, nx_char ch) {
    NX_STR_VIEW_ASSERT(self);

    for (nx_usize i = 0; i < self.len; ++i) {
        if (self.data[i] == ch) {
            return (nx_isize) i;
        }
    }
    return -1;
}

/* ========== trim ========== */

nx_StrView nx_str_view_trim_start(nx_StrView self) {
    NX_STR_VIEW_ASSERT(self);

    nx_usize i = 0;
    while (i < self.len && isspace(self.data[i])) {
        ++i;
    }

    return nx_str_view_new(self.data + i, self.len - i);
}

nx_StrView nx_str_view_trim_end(nx_StrView self) {
    NX_STR_VIEW_ASSERT(self);

    nx_usize i = self.len;
    while (i > 0 && isspace(self.data[i - 1])) {
        --i;
    }

    return nx_str_view_new(self.data, i);
}

nx_StrView nx_str_view_trim(nx_StrView self) {
    NX_STR_VIEW_ASSERT(self);

    return nx_str_view_trim_end(nx_str_view_trim_start(self));
}
/* ========== print ========== */

void nx_str_view_fprint(FILE *stream, nx_StrView self) {
    NX_ASSERT(stream);
    NX_STR_VIEW_ASSERT(self);

    const nx_usize len = self.len;
    const nx_char *p = self.data;

    if (len == 0) {
        return;
    }

    NX_ASSERT(p);

    fwrite(p, 1, len, stream);
}

void nx_str_view_fprintln(FILE *stream, nx_StrView self) {
    nx_str_view_fprint(stream, self);
    fputc('\n', stream);
}

void nx_str_view_print(nx_StrView self) {
    nx_str_view_fprint(stdout, self);
}

void nx_str_view_println(nx_StrView self) {
    nx_str_view_fprintln(stdout, self);
}
