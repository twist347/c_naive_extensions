#pragma once

#include "nx/core/assert.h"
#include "nx/core/type.h"

/* ========== macro helpers ========== */

#define NX_STRINGIFY_(x)   #x
/// expands x, then stringifies.
#define NX_STRINGIFY(x)    NX_STRINGIFY_(x)

#define NX_CONCAT_(a, b)   a##b
/// expands a and b, then concatenates.
#define NX_CONCAT(a, b)    NX_CONCAT_(a, b)

/* ========== array ========== */

/// element count for a C array (not a pointer).
#define NX_C_ARR_LEN(arr)    (sizeof((arr)) / sizeof((arr)[0]))

/* ========== misc ========== */

/// silences unused-variable warnings.
#define NX_UNUSED(val)    ((void) (val))

/// compile-time check that x is an lvalue (not a temporary).
#define NX_REQUIRE_LVALUE(x)    ((void) sizeof(&(x)))

/// type-safe swap using typeof.
#define NX_SWAP(a, b)            \
    do {                         \
        typeof(a) nx_tmp_ = (a); \
        (a) = (b);               \
        (b) = nx_tmp_;           \
    } while (0)

/* ========== abort helpers ========== */

/// aborts with "not implemented" message. use as placeholder for unfinished code.
#define NX_UNIMPLEMENTED() \
    nx_assert_fail("Unimplemented", "not implemented", __FILE__, __LINE__, __func__, nx_null)

#define NX_UNIMPLEMENTED_MSG(msg) \
    nx_assert_fail("Unimplemented", "not implemented", __FILE__, __LINE__, __func__, (msg))

#define NX_UNREACHABLE() \
    nx_assert_fail("Unreachable", "code should be unreachable", __FILE__, __LINE__, __func__, nx_null)
