#pragma once

#include "nx/core/cmp.h"
#include "nx/core/type.h"
#include "nx/ds/span.h"

// TODO: add equal_range

/// linear search. returns index of first match, or -1 if not found.
nx_isize nx_find(nx_CSpan s, const void *key, nx_cmp_fn cmp);
nx_bool nx_contains(nx_CSpan s, const void *key, nx_cmp_fn cmp);
nx_usize nx_count(nx_CSpan s, const void *key, nx_cmp_fn cmp);

/// binary search on sorted span. returns index of first element >= key.
nx_usize nx_lower_bound(nx_CSpan s, const void *key, nx_cmp_fn cmp);

/// binary search on sorted span. returns index of first element > key.
nx_usize nx_upper_bound(nx_CSpan s, const void *key, nx_cmp_fn cmp);

/// binary search on sorted span. returns index of match, or -1 if not found.
nx_isize nx_bsearch(nx_CSpan s, const void *key, nx_cmp_fn cmp);

/// returns index of first element satisfying pred, or -1.
nx_isize nx_find_if(nx_CSpan s, nx_predicate_fn pred);
nx_usize nx_count_if(nx_CSpan s, nx_predicate_fn pred);