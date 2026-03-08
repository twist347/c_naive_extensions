#pragma once

#include <stdio.h>

#include "nx/core/config.h"
#include "nx/core/type.h"

// TODO: add print funcs

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
} nx_StrView;

/* ========== assert ========== */

#if NX_DEBUG
    void nx_str_view_assert_(nx_StrView s);
#define NX_STR_VIEW_ASSERT(s) \
    do { nx_str_view_assert_((s)); } while (0)
#else
    #define NX_STR_VIEW_ASSERT(s) ((void) 0)
#endif

/* ========== lifetime ========== */

nx_StrView nx_str_view_new(const nx_char *data, nx_usize len);
nx_StrView nx_str_view_from_cstr(const nx_char *cstr);

/* ========== comparisons ========== */

nx_bool nx_str_view_eq(nx_StrView a, nx_StrView b);
nx_bool nx_str_view_not_eq(nx_StrView a, nx_StrView b);
nx_i32 nx_str_view_cmp(nx_StrView a, nx_StrView b); /* lexicographic */

/* ========== prefix/suffix ========== */

nx_bool nx_sv_starts_with(nx_StrView self, nx_StrView prefix);
nx_bool nx_sv_ends_with(nx_StrView self, nx_StrView suffix);

/* ========== slice ========== */

nx_StrView nx_str_view_sub(nx_StrView self, nx_usize pos, nx_usize len);

/* ========== search ========== */

nx_isize nx_str_view_find_sub(nx_StrView self, nx_StrView needle);
nx_isize nx_str_view_find_char(nx_StrView self, nx_char ch);

/* ========== trim ========== */

nx_StrView nx_str_view_trim_start(nx_StrView self);
nx_StrView nx_str_view_trim_end(nx_StrView self);
nx_StrView nx_str_view_trim(nx_StrView self);

/* ========== trim ========== */

void nx_str_view_fprint(FILE *stream, nx_StrView self);
void nx_str_view_fprintln(FILE *stream, nx_StrView self);

void nx_str_view_print(nx_StrView self);
void nx_str_view_println(nx_StrView self);
