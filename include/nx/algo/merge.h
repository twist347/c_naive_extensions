#pragma once

#include "nx/core/type.h"
#include "nx/core/cmp.h"
#include "nx/ds/span.h"

/// Merges two sorted spans into dst.
/// dst.len must equal a.len + b.len. All tsz must match.
/// Both a and b must be sorted according to cmp.
void nx_merge(nx_Span dst, nx_CSpan a, nx_CSpan b, nx_cmp_fn cmp);

/// In-place merge: given a span where [0, mid) and [mid, len) are each sorted,
/// produces a fully sorted span.
/// O(n) extra memory.
void nx_inplace_merge(nx_Span s, nx_usize mid, nx_cmp_fn cmp);
