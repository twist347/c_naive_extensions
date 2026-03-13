#pragma once

#include "nx/core/cmp.h"
#include "nx/core/type.h"
#include "nx/ds/span.h"

// TODO: add equal_range

nx_isize nx_find(nx_CSpan s, const void *key, nx_cmp_fn cmp);
nx_bool nx_contains(nx_CSpan s, const void *key, nx_cmp_fn cmp);
nx_usize nx_count(nx_CSpan s, const void *key, nx_cmp_fn cmp);

// requires sorted range by the same cmp
nx_usize nx_lower_bound(nx_CSpan s, const void *key, nx_cmp_fn cmp);
nx_usize nx_upper_bound(nx_CSpan s, const void *key, nx_cmp_fn cmp);
nx_isize nx_bsearch(nx_CSpan s, const void *key, nx_cmp_fn cmp);

nx_isize nx_find_if(nx_CSpan s, nx_predicate_fn pred);
nx_usize nx_count_if(nx_CSpan s, nx_predicate_fn pred);