#pragma once

#include "nx/core/type.h"
#include "nx/core/cmp.h"
#include "nx/ds/span.h"

// TODO: add ctx versions

void nx_sort(nx_Span s, nx_cmp_fn cmp);

void nx_sort_stable(nx_Span s, nx_cmp_fn cmp);

void nx_partial_sort(nx_Span s, nx_usize k, nx_cmp_fn cmp);

void nx_nth_element(nx_Span s, nx_usize nth, nx_cmp_fn cmp);

nx_bool nx_is_sorted(nx_CSpan s, nx_cmp_fn cmp);
