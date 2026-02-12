#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void nx_assert_fail(
    const char *type,
    const char *assertion,
    const char *file,
    int line,
    const char *func,
    const char *msg
);

#ifdef __cplusplus
}
#endif

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

#define NX_STATIC_ASSERT_MSG(x, msg)    static_assert((x), msg)
#define NX_STATIC_ASSERT(x)     NX_STATIC_ASSERT_MSG((x), "")
