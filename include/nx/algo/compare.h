#pragma once

#include "nx/core/type.h"
#include "nx/core/cmp.h"
#include "nx/ds/span.h"

/// element-wise comparison. spans must have same tsz. lengths may differ (unequal length => not equal).
nx_bool nx_equal(nx_CSpan a, nx_CSpan b, nx_cmp_fn cmp);
nx_bool nx_not_equal(nx_CSpan a, nx_CSpan b, nx_cmp_fn cmp);

// TODO: lexicographic_compare, mismatch
