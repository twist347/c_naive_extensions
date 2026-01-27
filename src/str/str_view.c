#include "nx/str/str_view.h"

#include <string.h>

#include "nx/core/assert.h"

#define NX_STR_VIEW_IS_VALID(sv_)                           \
    do {                                                    \
        NX_ASSERT((sv_).len == 0 || (sv_).data != NULL);    \
    } while (0)

nx_str_view nx_str_view_create(const char *data, size_t len) {
    return (nx_str_view){.data = data, .len = len};
}

nx_str_view nx_str_view_from_cstr(const char *cstr) {
    return nx_str_view_create(cstr, strlen(cstr));
}

bool nx_str_view_eq(nx_str_view a, nx_str_view b) {
    NX_STR_VIEW_IS_VALID(a);
    NX_STR_VIEW_IS_VALID(b);

    if (a.len != b.len) {
        return false;
    }

    if (a.len == 0) {
        return true;
    }

    return memcmp(a.data, b.data, a.len) == 0;
}

bool nx_str_view_not_eq(nx_str_view a, nx_str_view b) {
    return !nx_str_view_eq(a, b);
}

int nx_str_view_cmp(nx_str_view a, nx_str_view b) {
    NX_STR_VIEW_IS_VALID(a);
    NX_STR_VIEW_IS_VALID(b);

    const size_t n = a.len < b.len ? a.len : b.len;

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
