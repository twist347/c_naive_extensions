#pragma once

#include "nx/core/type.h"
#include "nx/core/cmp.h"
#include "nx/ds/span.h"

void nx_sort(nx_span s, nx_cmp cmp);
void nx_sort_ctx(nx_span s, nx_cmp cmp, void *ctx);

void nx_sort_stable(nx_span s, nx_cmp cmp);
void nx_sort_stable_ctx(nx_span s, nx_cmp cmp, void *ctx);

void nx_partial_sort(nx_span s, nx_usize k, nx_cmp cmp);

void nx_nth_element(nx_span s, nx_usize nth, nx_cmp cmp);

nx_bool nx_is_sorted_asc(nx_cspan s, nx_cmp cmp);
nx_bool nx_is_sorted_desc(nx_cspan s, nx_cmp cmp);
