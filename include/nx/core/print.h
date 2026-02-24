#pragma once

#include <stdio.h>
#include <inttypes.h>

#include "nx/ds/span.h"
#include "nx/core/type.h"

/* ========== format strings ========== */

#define NX_FMT_I8         "%" PRId8
#define NX_FMT_I16        "%" PRId16
#define NX_FMT_I32        "%" PRId32
#define NX_FMT_I64        "%" PRId64

#define NX_FMT_U8         "%" PRIu8
#define NX_FMT_U16        "%" PRIu16
#define NX_FMT_U32        "%" PRIu32
#define NX_FMT_U64        "%" PRIu64

#define NX_FMT_USIZE      "%zu"
#define NX_FMT_ISIZE      "%td"

#define NX_FMT_CHAR       "%c"
#define NX_FMT_BYTE       "%hhu"

#define NX_FMT_F32        "%g"
#define NX_FMT_F64        "%g"

#define NX_FMT_PTR        "%p"

#define NX_FMT_BOOL       "%s"
#define NX_BOOL_STR(b)    ((b) ? "true" : "false")

#define NX_FMT_HASH       NX_FMT_U64

/* ========== printf wrappers ========== */

typedef void (*nx_fprint_fn)(FILE *, const void *);

nx_i32 nx_printf(const char *fmt, ...);
nx_i32 nx_fprintf(FILE *stream, const char *fmt, ...);

/* ========== typed printers (void* for use as nx_fprint_fn) ========== */

void nx_fprint_i8(FILE *stream, const void *data);
void nx_fprint_i16(FILE *stream, const void *data);
void nx_fprint_i32(FILE *stream, const void *data);
void nx_fprint_i64(FILE *stream, const void *data);

void nx_fprint_u8(FILE *stream, const void *data);
void nx_fprint_u16(FILE *stream, const void *data);
void nx_fprint_u32(FILE *stream, const void *data);
void nx_fprint_u64(FILE *stream, const void *data);

void nx_fprint_usize(FILE *stream, const void *data);
void nx_fprint_isize(FILE *stream, const void *data);

void nx_fprint_bool(FILE *stream, const void *data);
void nx_fprint_byte(FILE *stream, const void *data);
void nx_fprint_char(FILE *stream, const void *data);

void nx_fprint_f32(FILE *stream, const void *data);
void nx_fprint_f64(FILE *stream, const void *data);

void nx_fprint_ptr(FILE *stream, const void *data);

/* ========== span printing ========== */

void nx_fprintln_cspan(FILE *stream, nx_cspan s, nx_fprint_fn f);
void nx_fprintln_span(FILE *stream, nx_span s, nx_fprint_fn f);

void nx_println_cspan(nx_cspan s, nx_fprint_fn f);
void nx_println_span(nx_span s, nx_fprint_fn f);

/* ========== debug ========== */

/// nx_dbg(x) => prints "x = <value>\n" with auto format
#define nx_dbg(x) _Generic((x),                           \
    nx_i8:    nx_printf(#x " = " NX_FMT_I8    "\n", (x)), \
    nx_i16:   nx_printf(#x " = " NX_FMT_I16   "\n", (x)), \
    nx_i32:   nx_printf(#x " = " NX_FMT_I32   "\n", (x)), \
    nx_i64:   nx_printf(#x " = " NX_FMT_I64   "\n", (x)), \
    nx_u8:    nx_printf(#x " = " NX_FMT_U8    "\n", (x)), \
    nx_u16:   nx_printf(#x " = " NX_FMT_U16   "\n", (x)), \
    nx_u32:   nx_printf(#x " = " NX_FMT_U32   "\n", (x)), \
    nx_u64:   nx_printf(#x " = " NX_FMT_U64   "\n", (x)), \
    nx_f32:   nx_printf(#x " = " NX_FMT_F32   "\n", (x)), \
    nx_f64:   nx_printf(#x " = " NX_FMT_F64   "\n", (x))  \
)

#define nx_fdbg(stream, x) _Generic((x),                             \
    nx_i8:    nx_fprintf((stream), #x " = " NX_FMT_I8    "\n", (x)), \
    nx_i16:   nx_fprintf((stream), #x " = " NX_FMT_I16   "\n", (x)), \
    nx_i32:   nx_fprintf((stream), #x " = " NX_FMT_I32   "\n", (x)), \
    nx_i64:   nx_fprintf((stream), #x " = " NX_FMT_I64   "\n", (x)), \
    nx_u8:    nx_fprintf((stream), #x " = " NX_FMT_U8    "\n", (x)), \
    nx_u16:   nx_fprintf((stream), #x " = " NX_FMT_U16   "\n", (x)), \
    nx_u32:   nx_fprintf((stream), #x " = " NX_FMT_U32   "\n", (x)), \
    nx_u64:   nx_fprintf((stream), #x " = " NX_FMT_U64   "\n", (x)), \
    nx_f32:   nx_fprintf((stream), #x " = " NX_FMT_F32   "\n", (x)), \
    nx_f64:   nx_fprintf((stream), #x " = " NX_FMT_F64   "\n", (x))  \
)
