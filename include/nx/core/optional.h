#pragma once

#include "nx/core/type.h"
#include "nx/core/assert.h"
#include "nx/core/util.h"

/// Define an optional type for T:
///     NX_DEF_OPT_TYPE(nx_OptI32, nx_i32);
///
/// Produces:
///     typedef struct { nx_bool has_val; nx_i32 val; } nx_OptI32;
///

#define NX_DEF_OPT_TYPE(name, T) \
    typedef struct {             \
        nx_bool has_val;         \
        T val;                   \
    } name

/* ========== constructors ========== */

#define NX_OPT_SOME(name, v)    ((name){.has_val = true,  .val = (v)})
#define NX_OPT_NONE(name)       ((name){.has_val = false})

/* ========== queries ========== */

#define NX_OPT_HAS_VAL(opt)    ((opt).has_val)
#define NX_OPT_IS_NONE(opt)    (!(opt).has_val)

/// requires lvalue. aborts if empty (always checked, even in release).
#define NX_OPT_UNWRAP(opt_lvalue)     \
    (NX_REQUIRE_LVALUE(opt_lvalue),   \
     NX_VERIFY((opt_lvalue).has_val), \
     (opt_lvalue).val)

/// return val if present, otherwise evaluate to default_val.
#define NX_OPT_UNWRAP_OR(opt_lvalue, default_val) \
    (NX_REQUIRE_LVALUE(opt_lvalue),               \
     (opt_lvalue).has_val ? (opt_lvalue).val : (default_val))

/// access val without checking. caller's responsibility.
#define NX_OPT_UNWRAP_UNCHECKED(opt)    ((opt).val)

/* ========== common optional types ========== */

NX_DEF_OPT_TYPE(nx_OptI8,    nx_i8);
NX_DEF_OPT_TYPE(nx_OptI16,   nx_i16);
NX_DEF_OPT_TYPE(nx_OptI32,   nx_i32);
NX_DEF_OPT_TYPE(nx_OptI64,   nx_i64);

NX_DEF_OPT_TYPE(nx_OptU8,    nx_u8);
NX_DEF_OPT_TYPE(nx_OptU16,   nx_u16);
NX_DEF_OPT_TYPE(nx_OptU32,   nx_u32);
NX_DEF_OPT_TYPE(nx_OptU64,   nx_u64);

NX_DEF_OPT_TYPE(nx_OptUsize, nx_usize);
NX_DEF_OPT_TYPE(nx_OptIsize, nx_isize);

NX_DEF_OPT_TYPE(nx_OptF32,   nx_f32);
NX_DEF_OPT_TYPE(nx_OptF64,   nx_f64);

NX_DEF_OPT_TYPE(nx_OptBool,  nx_bool);
NX_DEF_OPT_TYPE(nx_OptChar,  nx_char);
NX_DEF_OPT_TYPE(nx_OptByte,  nx_byte);

NX_DEF_OPT_TYPE(nx_OptPtr,   void *);
NX_DEF_OPT_TYPE(nx_OptCPtr,  const void *);
