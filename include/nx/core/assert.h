#pragma once

[[noreturn]] void nx_assert_fail(
    const char *type,
    const char *assertion,
    const char *file,
    int line,
    const char *func,
    const char *msg
);

#if defined(NDEBUG)
    #define NX_DEBUG 0
#else
    #define NX_DEBUG 1
#endif

#if NX_DEBUG

    #define NX_ASSERT(expr)                                                                          \
        ((void)(0 && (expr)),                                                                        \
         (void)((expr) ||                                                                            \
                (nx_assert_fail("Assertion failed", #expr, __FILE__, __LINE__, __func__, NULL), 0)))

    #define NX_ASSERT_MSG(expr, msg) \
        ((void)(0 && (expr)), \
         (void)((expr) || (nx_assert_fail("Assertion", #expr, __FILE__, __LINE__, __func__, (msg)), 0)))

#else
    #define NX_ASSERT(expr)    ((void)(0 && (expr)))
    #define NX_ASSERT_MSG(expr, msg) ((void)(0 && (expr) && (msg)))
#endif
