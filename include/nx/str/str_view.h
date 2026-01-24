#pragma once

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nx_str_view {
    const char *data;
    size_t len;
} nx_str_view;

nx_str_view nx_str_view_make(const char *data, size_t len);
nx_str_view nx_str_view_from_cstr(const char *cstr);

/* ---------- comparisons ---------- */

bool nx_str_view_eq(nx_str_view a, nx_str_view b);
bool nx_str_view_not_eq(nx_str_view a, nx_str_view b);
int nx_str_view_cmp(nx_str_view a, nx_str_view b); /* lexicographic */

/* ---------- prefix/suffix ---------- */

bool nx_sv_starts_with(nx_str_view s, nx_str_view prefix);
bool nx_sv_ends_with(nx_str_view s, nx_str_view suffix);

/* ---------- slice ---------- */

nx_str_view nx_str_view_substr(nx_str_view s, size_t pos, size_t len);

/* ---------- search ---------- */

ptrdiff_t nx_str_view_find(nx_str_view s, nx_str_view needle);

#ifdef __cplusplus
}
#endif