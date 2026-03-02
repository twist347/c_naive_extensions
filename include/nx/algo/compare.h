#pragma once

#include "nx/core/type.h"
#include "nx/core/cmp.h"
#include "nx/ds/span.h"

nx_bool nx_equal(nx_CSpan a, nx_CSpan b, nx_cmp_fn cmp);
nx_bool nx_not_equal(nx_CSpan a, nx_CSpan b, nx_cmp_fn cmp);

// lexicographic_compare, mismatch
