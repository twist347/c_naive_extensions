#pragma once

#include <stddef.h>
#include <stdint.h>

/* ========== types aliases ========== */

// TODO: _Float32, _Float64

#define nx_null          nullptr

typedef int8_t           nx_i8;
typedef int16_t          nx_i16;
typedef int32_t          nx_i32;
typedef int64_t          nx_i64;

typedef uint8_t          nx_u8;
typedef uint16_t         nx_u16;
typedef uint32_t         nx_u32;
typedef uint64_t         nx_u64;

typedef size_t           nx_usize;
typedef ptrdiff_t        nx_isize;

typedef uintptr_t        nx_uptr;
typedef intptr_t         nx_iptr;

typedef bool             nx_bool;
typedef unsigned char    nx_byte;
typedef char             nx_char;

typedef float            nx_f32;
typedef double           nx_f64;

static_assert(sizeof(float) == 4, "nx_f32 requires 4-byte float");
static_assert(sizeof(double) == 8, "nx_f64 requires 8-byte double");

/* ========== literal macros ========== */

#define NX_I8_C(x)      INT8_C(x)
#define NX_I16_C(x)     INT16_C(x)
#define NX_I32_C(x)     INT32_C(x)
#define NX_I64_C(x)     INT64_C(x)

#define NX_U8_C(x)      UINT8_C(x)
#define NX_U16_C(x)     UINT16_C(x)
#define NX_U32_C(x)     UINT32_C(x)
#define NX_U64_C(x)     UINT64_C(x)

#define NX_F32_C(x)     x##f
#define NX_F64_C(x)     x

#define NX_USIZE_C(x)   ((nx_usize) (x))
#define NX_ISIZE_C(x)   ((nx_isize) (x))
