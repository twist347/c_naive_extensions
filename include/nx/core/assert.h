#pragma once

#ifndef NX_ENABLE_ASSERT
  #define NX_ENABLE_ASSERT 1
#endif

// must be expression-like
#if NX_ENABLE_ASSERT
  #include <assert.h>
  #define NX_ASSERT(expr)    assert(expr)
#else
  #define NX_ASSERT(expr)    ((void) 0)
#endif

#ifdef __cplusplus
    #define NX_STATIC_ASSERT_MSG(x, msg) static_assert((x), msg)
#else
    #define NX_STATIC_ASSERT_MSG(x, msg) _Static_assert((x), msg)
#endif
#define NX_STATIC_ASSERT(x)     NX_STATIC_ASSERT_MSG((x), "")
