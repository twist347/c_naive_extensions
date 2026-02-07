#pragma once

#include <stdio.h>
#include <inttypes.h>

#include "nx/core/span.h"
#include "nx/core/type.h"

#ifdef __cplusplus
extern "C" {
#endif

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

#define NX_FMT_FLT        "%g"
#define NX_FMT_DBL        "%g"

#define NX_FMT_F32        "%g"
#define NX_FMT_F64        "%g"

#define NX_FMT_BOOL       "%s"
#define NX_BOOL_STR(b)    ((b) ? "true" : "false")

#define NX_FMT_HASH       NX_FMT_U64

typedef void (*nx_fprint_fn)(FILE *, const void *);

nx_i32 nx_fprintf(FILE *stream, const char *fmt, ...);

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

void nx_fprint_flt(FILE *stream, const void *data);
void nx_fprint_dbl(FILE *stream, const void *data);

void nx_fprint_f32(FILE *stream, const void *data);
void nx_fprint_f64(FILE *stream, const void *data);

void nx_fprintln_span(FILE *stream, nx_span s, nx_fprint_fn f);
void nx_fprintln_cspan(FILE *stream, nx_cspan s, nx_fprint_fn f);

#ifdef __cplusplus
}
#endif

