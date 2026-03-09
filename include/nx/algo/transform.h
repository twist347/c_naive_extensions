#pragma once

#include "nx/core/type.h"
#include "nx/core/cmp.h"
#include "nx/ds/span.h"

void nx_fill(nx_Span s, const void *elem);
void nx_reverse(nx_Span s);
void nx_rotate(nx_Span s, nx_usize mid);

typedef void (*nx_transform_fn)(void *dst, const void *src);
void nx_transform(nx_Span dst, nx_CSpan src, nx_transform_fn fn);

/* ========== copy ========== */

typedef nx_bool (*nx_predicate_fn)(const void *elem);

void nx_copy(nx_Span dst, nx_CSpan src);
nx_usize nx_copy_if(nx_Span dst, nx_CSpan src, nx_predicate_fn pred);

/* ========== replace ========== */

nx_usize nx_replace(nx_Span s, const void *old_val, const void *new_val, nx_cmp_fn cmp); // returns replaced count
nx_usize nx_replace_if(nx_Span s, nx_predicate_fn pred, const void *new_val);            // returns replaced count

/* ========== generate ========== */

typedef void (*nx_generate_fn)(void *out);

void nx_generate(nx_Span s, nx_generate_fn gen);

/* ========== for_each ========== */

typedef void (*nx_for_each_fn)(void *elem);
typedef void (*nx_for_each_c_fn)(const void *elem);

void nx_for_each(nx_Span s, nx_for_each_fn fn);
void nx_for_each_c(nx_CSpan s, nx_for_each_c_fn fn);

/* ========== ? ========== */

nx_usize nx_remove(nx_Span s, const void *key, nx_cmp_fn cmp);
nx_usize nx_remove_if(nx_Span s, nx_predicate_fn pred);
nx_usize nx_unique(nx_Span s, nx_cmp_fn cmp);

