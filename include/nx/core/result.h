#pragma once

#include "nx/core/status.h"
#include "nx/core/panic.h"
#include "nx/core/util.h"

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

/// requires lvalue
#define NX_RES_UNWRAP(res_lvalue)                \
    (NX_REQUIRE_LVALUE(res_lvalue),              \
     NX_VERIFY((res_lvalue).st == NX_STATUS_OK), \
     (res_lvalue).val)

#define NX_RES_UNWRAP_INTO(out_lvalue, expr) \
    do {                                     \
        typeof(expr) _r_ = (expr);           \
        NX_VERIFY(_r_.st == NX_STATUS_OK);   \
        (out_lvalue) = _r_.val;              \
    } while (0)

#define NX_RES_UNWRAP_UNCHECKED(res) \
    ((res).val)
