#pragma once

#include <stdio.h>

#include "nx/core/print.h"
#include "nx/core/span.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*nx_fprint_func)(FILE *, const void *);

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

void nx_fprintln_span(FILE *stream, nx_span s, nx_fprint_func f);
void nx_fprintln_cspan(FILE *stream, nx_cspan s, nx_fprint_func f);

#ifdef __cplusplus
}
#endif

