#pragma once

#include "nx/core/type.h"
#include "nx/core/cmp.h"
#include "nx/ds/span.h"

nx_isize nx_find(nx_cspan s, const void *key, nx_cmp cmp);
nx_bool nx_contains(nx_cspan s, const void *key, nx_cmp cmp);
nx_usize nx_count(nx_cspan s, const void *key, nx_cmp cmp);

// requires sorted range by the same cmp
nx_usize nx_lower_bound(nx_cspan s, const void *key, nx_cmp cmp);
nx_usize nx_upper_bound(nx_cspan s, const void *key, nx_cmp cmp);
nx_isize nx_bsearch(nx_cspan s, const void *key, nx_cmp cmp);

typedef nx_bool (*nx_predicate_fn)(const void *elem);
nx_isize nx_find_if(nx_cspan s, nx_predicate_fn pred);
nx_usize nx_count_if(nx_cspan s, nx_predicate_fn pred);