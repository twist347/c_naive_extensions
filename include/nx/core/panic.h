#pragma once

#include "nx/core/assert.h"

#define NX_VERIFY(cond) \
    ((void)(0 && (cond)), \
     (void)((cond) || (nx_assert_fail("Verify", #cond, __FILE__, __LINE__, __func__, nullptr), 0)))

#define NX_VERIFY_MSG(cond, msg) \
    ((void)(0 && (cond)), \
     (void)((cond) || (nx_assert_fail("Verify", #cond, __FILE__, __LINE__, __func__, (msg)), 0)))

#define NX_PANIC() \
    nx_assert_fail("Panic", "explicit panic", __FILE__, __LINE__, __func__, nullptr)

#define NX_PANIC_MSG(msg) \
    nx_assert_fail("Panic", "explicit panic", __FILE__, __LINE__, __func__, (msg))

#define NX_UNIMPLEMENTED() \
    nx_assert_fail("Unimplemented", __func__, __FILE__, __LINE__, __func__, nullptr)

#define NX_UNREACHABLE() \
    nx_assert_fail("Unreachable", "code should be unreachable", __FILE__, __LINE__, __func__, nullptr)
