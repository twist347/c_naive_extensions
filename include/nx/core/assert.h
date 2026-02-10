#pragma once

#if defined(NDEBUG)
  #define NX_DEBUG 0
#else
  #define NX_DEBUG 1
#endif

#if NX_DEBUG
    #include <assert.h>
    #define NX_ASSERT(expr)    ((void)(0 && (expr)), assert(expr))
#else
    #define NX_ASSERT(expr)    ((void)(0 && (expr)))
#endif

#define NX_STATIC_ASSERT_MSG(x, msg)    static_assert((x), msg)
#define NX_STATIC_ASSERT(x)     NX_STATIC_ASSERT_MSG((x), "")
