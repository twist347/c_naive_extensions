#pragma once

#define NX_VERSION_MAJOR 0
#define NX_VERSION_MINOR 1
#define NX_VERSION_STR   "0.1"

#if defined(NDEBUG)
    #define NX_DEBUG 0
#else
    #define NX_DEBUG 1
#endif
