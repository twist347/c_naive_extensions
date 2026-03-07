#pragma once

#include "nx/core/config.h"

[[noreturn]] void nx_assert_fail(
    const char *type,
    const char *assertion,
    const char *file,
    int line,
    const char *func,
    const char *msg
);

#if NX_DEBUG

    #define NX_ASSERT(expr)   \
        ((void)(0 && (expr)), \
         (void)((expr) ||     \
                (nx_assert_fail("Assertion failed", #expr, __FILE__, __LINE__, __func__, nullptr), 0)))

    #define NX_ASSERT_MSG(expr, msg) \
        ((void)(0 && (expr)),        \
         (void)((expr) || (nx_assert_fail("Assertion failed", #expr, __FILE__, __LINE__, __func__, (msg)), 0)))

#else
    #define NX_ASSERT(expr)    ((void)(0 && (expr)))
    #define NX_ASSERT_MSG(expr, msg) ((void)(0 && (expr)), (void)(0 && (msg)))
#endif

#define NX_VERIFY(cond)   \
    ((void)(0 && (cond)), \
     (void)((cond) || (nx_assert_fail("Verify failed", #cond, __FILE__, __LINE__, __func__, nullptr), 0)))

#define NX_VERIFY_MSG(cond, msg) \
    ((void)(0 && (cond)),        \
     (void)((cond) || (nx_assert_fail("Verify failed", #cond, __FILE__, __LINE__, __func__, (msg)), 0)))

#define NX_PANIC() \
    nx_assert_fail("Panic", "explicit panic", __FILE__, __LINE__, __func__, nullptr)

#define NX_PANIC_MSG(msg) \
    nx_assert_fail("Panic", "explicit panic", __FILE__, __LINE__, __func__, (msg))
