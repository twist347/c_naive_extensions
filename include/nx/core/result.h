#pragma once

#include "nx/core/status.h"
#include "nx/core/type.h"
#include "nx/core/assert.h"
#include "nx/core/panic.h"

#define NX_DEF_RES_TYPE(name, T)    \
    typedef struct {                \
        nx_status st;               \
        T val;                      \
    } name

/* ========== constructors ========== */

#define NX_RES_NEW_OK(name, v)     ((name){.st = NX_STATUS_OK, .val = (v)})
#define NX_RES_NEW_ERR(name, e)    ((name){.st = (e)})

/* ========== queries ========== */

#define NX_RES_IS_OK(res) \
    ((res).st == NX_STATUS_OK)

#define NX_RES_IS_ERR(res) \
    ((res).st != NX_STATUS_OK)

#define NX_RES_ERR(res) \
    ((res).st)

/* ========== value extraction ========== */

#define NX_RES_UNWRAP(res) \
    (NX_VERIFY((res).st == NX_STATUS_OK), (res).val)

#define NX_RES_UNWRAP_UNCHECKED(res) \
    ((res).val)
