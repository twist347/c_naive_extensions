#pragma once

#include "nx/core/status.h"
#include "nx/core/type.h"
#include "nx/core/assert.h"

#define NX_DEF_RESULT_TYPE(name, T)    \
    typedef struct name {              \
        nx_status st;                  \
        T val;                         \
    } name

#ifdef __cplusplus
    #define NX_RES_OK(name, v)     (name{NX_STATUS_OK, (v)})
    #define NX_RES_ERR(name, e)    (name{(e), {}})
#else
    #define NX_RES_OK(name, v)     ((name){.st = NX_STATUS_OK, .val = (v)})
    #define NX_RES_ERR(name, e)    ((name){.st = (e), .val = (0) })
#endif

#define NX_RES_IS_OK(r)    \
    ((r).st == NX_STATUS_OK)

#define NX_RES_GET_VAL(res)    \
    (NX_ASSERT((res).st == NX_STATUS_OK), (res).val)

#define NX_RES_GET_ERR(res)    \
    (NX_ASSERT((res).st != NX_STATUS_OK), (res).st)
