#pragma once

#include "nx/core/type.h"
#include "nx/core/cmp.h"
#include "nx/ds/span.h"

#ifdef __cplusplus
extern "C" {
#endif

nx_isize nx_find(nx_cspan s, const void *key, nx_cmp cmp);
nx_bool nx_contains(nx_cspan s, const void *key, nx_cmp cmp);
nx_usize nx_count(nx_cspan s, const void *key, nx_cmp cmp);

// requires sorted range by the same cmp
nx_isize nx_lower_bound(nx_cspan s, const void *key, nx_cmp cmp);
nx_isize nx_upper_bound(nx_cspan s, const void *key, nx_cmp cmp);
nx_isize nx_bsearch(nx_cspan s, const void *key, nx_cmp cmp);

#ifdef __cplusplus
}
#endif
