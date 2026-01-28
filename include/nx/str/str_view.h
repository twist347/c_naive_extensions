#pragma once

#include "nx/core/type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nx_str_view {
    const nx_char *data;
    nx_usize len;
} nx_str_view;

nx_str_view nx_str_view_new(const nx_char *data, nx_usize len);
nx_str_view nx_str_view_from_cstr(const nx_char *cstr);

/* ---------- comparisons ---------- */

nx_bool nx_str_view_eq(nx_str_view a, nx_str_view b);
nx_bool nx_str_view_not_eq(nx_str_view a, nx_str_view b);
nx_i32 nx_str_view_cmp(nx_str_view a, nx_str_view b); /* lexicographic */

/* ---------- prefix/suffix ---------- */

nx_bool nx_sv_starts_with(nx_str_view s, nx_str_view prefix);
nx_bool nx_sv_ends_with(nx_str_view s, nx_str_view suffix);

/* ---------- slice ---------- */

nx_str_view nx_str_view_substr(nx_str_view s, nx_usize pos, nx_usize len);

/* ---------- search ---------- */

nx_isize nx_str_view_find(nx_str_view s, nx_str_view needle);

#ifdef __cplusplus
}
#endif