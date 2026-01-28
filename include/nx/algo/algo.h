#pragma once

#include "nx/core/type.h"
#include "nx/core/span.h"
#include "nx/core/cmp.h"

typedef struct {
    nx_isize min;
    nx_isize max;
} nx_minmax;

/* ---------- sort/order ---------- */

void nx_sort(nx_span s, nx_cmp cmp);
nx_bool nx_is_sorted(nx_cspan s, nx_cmp cmp);

/* ---------- search ---------- */

nx_isize nx_find(nx_cspan s, const void *key, nx_cmp cmp);
nx_bool nx_contains(nx_cspan s, const void *key, nx_cmp cmp);
nx_usize nx_count(nx_cspan s, const void *key, nx_cmp cmp);

// requires sorted range by the same cmp
nx_isize nx_lower_bound(nx_cspan s, const void *key, nx_cmp cmp);
nx_isize nx_upper_bound(nx_cspan s, const void *key, nx_cmp cmp);
nx_isize nx_bsearch(nx_cspan s, const void *key, nx_cmp cmp);

/* ---------- min/max ---------- */

nx_isize nx_min_element(nx_cspan s, nx_cmp cmp);
nx_isize nx_max_element(nx_cspan s, nx_cmp cmp);
nx_minmax nx_minmax_element(nx_cspan s, nx_cmp cmp);

/* ---------- comparison ---------- */

nx_bool nx_equal(nx_cspan a, nx_cspan b, nx_cmp cmp);
nx_bool nx_not_equal(nx_cspan a, nx_cspan b, nx_cmp cmp);

/* ---------- ? ---------- */

void nx_fill(nx_span s, const void *elem);
void nx_reverse(nx_span s);
void nx_swap_elements(nx_span s, nx_usize i, nx_usize j);

