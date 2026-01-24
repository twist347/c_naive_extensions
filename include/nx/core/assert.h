#pragma once

#if defined(NX_NO_ASSERT)
    #define NX_ASSERT(expr)    ((void) 0)
#else
    #include <assert.h>
    #define NX_ASSERT(expr)    assert(expr)
#endif

#define NX_STATIC_ASSERT_MSG(x, msg)    _Static_assert((x), (msg))
#define NX_STATIC_ASSERT(x)     NX_STATIC_ASSERT_MSG((x), "")
