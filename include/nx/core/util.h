#pragma once

#include "nx/core/assert.h"

#define NX_C_ARR_LEN(arr)    (sizeof((arr)) / sizeof((arr)[0]))

#define NX_UNUSED(val)    ((void) (val))

#define NX_REQUIRE_LVALUE(x)    ((void) sizeof(&(x)))

#define NX_UNIMPLEMENTED() \
    nx_assert_fail("Unimplemented", "not implemented", __FILE__, __LINE__, __func__, nullptr)

#define NX_UNREACHABLE() \
    nx_assert_fail("Unreachable", "code should be unreachable", __FILE__, __LINE__, __func__, nullptr)
