#pragma once

#include <stdio.h>

#include "nx/core/type.h"
#include "nx/io/fmt.h"

/* ========== formatted print ========== */

/// thin wrappers over printf/fprintf/snprintf.
nx_i32 nx_printf(const char *fmt, ...);
nx_i32 nx_fprintf(FILE *stream, const char *fmt, ...);
nx_i32 nx_snprintf(nx_char *buf, nx_usize size, const char *fmt, ...);

/* ========== typed printers (void* for use as nx_fprint_fn) ========== */

/// callback type for printing a single element. used by span/vec/arr print.
typedef void (*nx_fprint_fn)(FILE *, const void *);

void nx_fprint_i8(FILE *stream, const void *data);
void nx_fprint_i16(FILE *stream, const void *data);
void nx_fprint_i32(FILE *stream, const void *data);
void nx_fprint_i64(FILE *stream, const void *data);

void nx_fprint_u8(FILE *stream, const void *data);
void nx_fprint_u16(FILE *stream, const void *data);
void nx_fprint_u32(FILE *stream, const void *data);
void nx_fprint_u64(FILE *stream, const void *data);

void nx_fprint_f32(FILE *stream, const void *data);
void nx_fprint_f64(FILE *stream, const void *data);

void nx_fprint_usize(FILE *stream, const void *data);
void nx_fprint_isize(FILE *stream, const void *data);

void nx_fprint_byte(FILE *stream, const void *data);
void nx_fprint_char(FILE *stream, const void *data);
void nx_fprint_bool(FILE *stream, const void *data);

void nx_fprint_str(FILE *stream, const void *data);

void nx_fprint_ptr(FILE *stream, const void *data);