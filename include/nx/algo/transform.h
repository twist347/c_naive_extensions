#pragma once

#include "nx/core/type.h"
#include "nx/core/cmp.h"
#include "nx/ds/span.h"

void nx_fill(nx_Span s, const void *elem);
void nx_reverse(nx_Span s);

/// left-rotate: moves [0, mid) to the end. result is [mid..len, 0..mid).
void nx_rotate(nx_Span s, nx_usize mid);

/// applies fn to each element: dst[i] = fn(src[i]). dst and src must have same len and tsz.
void nx_transform(nx_Span dst, nx_CSpan src, nx_transform_fn fn);

/* ========== copy ========== */

/// copies src into dst (memmove-safe). same len and tsz required.
void nx_copy(nx_Span dst, nx_CSpan src);

/// copies elements satisfying pred into dst. returns number of elements copied.
nx_usize nx_copy_if(nx_Span dst, nx_CSpan src, nx_predicate_fn pred);

/* ========== replace ========== */

/// replaces all occurrences of old_val with new_val. returns replaced count.
nx_usize nx_replace(nx_Span s, const void *old_val, const void *new_val, nx_cmp_fn cmp);

/// replaces elements satisfying pred with new_val. returns replaced count.
nx_usize nx_replace_if(nx_Span s, nx_predicate_fn pred, const void *new_val);

/* ========== generate ========== */

void nx_generate(nx_Span s, nx_generate_fn gen);

/* ========== for_each ========== */

void nx_for_each(nx_Span s, nx_for_each_fn fn);
void nx_for_each_c(nx_CSpan s, nx_for_each_c_fn fn);

/* ========== remove / unique ========== */

/// moves non-matching elements to the front. returns new logical length.
nx_usize nx_remove(nx_Span s, const void *key, nx_cmp_fn cmp);
nx_usize nx_remove_if(nx_Span s, nx_predicate_fn pred);

/// removes consecutive duplicates. span must be sorted. returns new logical length.
nx_usize nx_unique(nx_Span s, nx_cmp_fn cmp);

