#pragma once

#include "nx/core/assert.h"
#include "nx/core/type.h"

/* ========== macro helpers ========== */

#define NX_STRINGIFY_(x)   #x
#define NX_STRINGIFY(x)    NX_STRINGIFY_(x)

#define NX_CONCAT_(a, b)   a##b
#define NX_CONCAT(a, b)    NX_CONCAT_(a, b)

/* ========== array ========== */

#define NX_C_ARR_LEN(arr)    (sizeof((arr)) / sizeof((arr)[0]))

/* ========== misc ========== */

#define NX_UNUSED(val)    ((void) (val))

#define NX_REQUIRE_LVALUE(x)    ((void) sizeof(&(x)))

#define NX_SWAP(a, b)            \
    do {                         \
        typeof(a) nx_tmp_ = (a); \
        (a) = (b);               \
        (b) = nx_tmp_;           \
    } while (0)

/* ========== abort helpers ========== */

#define NX_UNIMPLEMENTED() \
    nx_assert_fail("Unimplemented", "not implemented", __FILE__, __LINE__, __func__, nx_null)

#define NX_UNIMPLEMENTED_MSG(msg) \
    nx_assert_fail("Unimplemented", "not implemented", __FILE__, __LINE__, __func__, (msg))

#define NX_UNREACHABLE() \
    nx_assert_fail("Unreachable", "code should be unreachable", __FILE__, __LINE__, __func__, nx_null)
