#pragma once

#include "nx/core/type.h"

/* Contract:
 * - Invalid usage (null, bounds) is a programmer error guarded by
 *   NX_ASSERT. Release behavior is unspecified.
 *
 * - Non-owning view: does not allocate or free memory.
 *   The referenced data must outlive the view.
 *
 * - Null data --> zero length
 *
 * - NOT null-terminated: len bytes from data, no '\0' assumed
 *
 * - Passed by value (small struct, no heap allocation)
 *
 * - No overflow checking
 */

typedef struct {
    const nx_char *data;
    nx_usize len;
} nx_str_view;

/* ========== assert ========== */

#if NX_DEBUG
    void nx_str_view_assert_(nx_str_view s);
#define NX_STR_VIEW_ASSERT(s) \
    do { nx_str_view_assert_((s)); } while (0)
#else
    #define NX_STR_VIEW_ASSERT(s) ((void) 0)
#endif

/* ========== lifetime ========== */

nx_str_view nx_str_view_new(const nx_char *data, nx_usize len);
nx_str_view nx_str_view_from_cstr(const nx_char *cstr);

/* ========== comparisons ========== */

nx_bool nx_str_view_eq(nx_str_view a, nx_str_view b);
nx_bool nx_str_view_not_eq(nx_str_view a, nx_str_view b);
nx_i32 nx_str_view_cmp(nx_str_view a, nx_str_view b); /* lexicographic */

/* ========== prefix/suffix ========== */

nx_bool nx_sv_starts_with(nx_str_view s, nx_str_view prefix);
nx_bool nx_sv_ends_with(nx_str_view s, nx_str_view suffix);

/* ========== slice ========== */

nx_str_view nx_str_view_sub(nx_str_view s, nx_usize pos, nx_usize len);

/* ========== search ========== */

nx_isize nx_str_view_find_sub(nx_str_view s, nx_str_view needle);
nx_isize nx_str_view_find_char(nx_str_view s, nx_char ch);

/* ========== trim ========== */

nx_str_view nx_str_view_trim_start(nx_str_view s);
nx_str_view nx_str_view_trim_end(nx_str_view s);
nx_str_view nx_str_view_trim(nx_str_view s);
