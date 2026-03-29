#pragma once

#include "nx/core/type.h"
#include "nx/core/cmp.h"
#include "nx/ds/span.h"

// TODO: add ctx versions

/// quicksort (wraps qsort). O(n log n) average, not stable.
void nx_sort(nx_Span s, nx_cmp_fn cmp);

/// merge sort. O(n log n), stable, O(n) extra memory.
void nx_sort_stable(nx_Span s, nx_cmp_fn cmp);

/// sorts the first k elements. elements after k are unspecified.
void nx_partial_sort(nx_Span s, nx_usize k, nx_cmp_fn cmp);

/// places the nth smallest element at index nth.
/// elements before nth are <= it, elements after are >=.
void nx_nth_element(nx_Span s, nx_usize nth, nx_cmp_fn cmp);

nx_bool nx_is_sorted(nx_CSpan s, nx_cmp_fn cmp);
