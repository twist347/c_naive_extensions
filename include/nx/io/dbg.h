#pragma once

#include "nx/io/print.h"

/* ========== debug ========== */

#define nx_dbg(x)                         \
    _Generic((x),                         \
        nx_i8:              nx_dbg_i8_,   \
        nx_u8:              nx_dbg_u8_,   \
        nx_u16:             nx_dbg_u16_,  \
        nx_i16:             nx_dbg_i16_,  \
        nx_i32:             nx_dbg_i32_,  \
        nx_i64:             nx_dbg_i64_,  \
        nx_u32:             nx_dbg_u32_,  \
        nx_u64:             nx_dbg_u64_,  \
        nx_f32:             nx_dbg_f32_,  \
        nx_f64:             nx_dbg_f64_,  \
        nx_bool:            nx_dbg_bool_, \
        nx_char *:          nx_dbg_cstr_, \
        const nx_char *:    nx_dbg_cstr_  \
    )(stdout, __FILE__, __LINE__, #x, (x))

#define nx_fdbg(stream, x)                \
    _Generic((x),                         \
        nx_i8:              nx_dbg_i8_,   \
        nx_i16:             nx_dbg_i16_,  \
        nx_i32:             nx_dbg_i32_,  \
        nx_i64:             nx_dbg_i64_,  \
        nx_u8:              nx_dbg_u8_,   \
        nx_u16:             nx_dbg_u16_,  \
        nx_u32:             nx_dbg_u32_,  \
        nx_u64:             nx_dbg_u64_,  \
        nx_f32:             nx_dbg_f32_,  \
        nx_f64:             nx_dbg_f64_,  \
        nx_bool:            nx_dbg_bool_, \
        nx_char *:          nx_dbg_cstr_, \
        const nx_char *:    nx_dbg_cstr_  \
    )((stream), __FILE__, __LINE__, #x, (x))

/* ========== internal helpers (not part of public API) ========== */

void nx_dbg_i8_(FILE *stream, const char *file, int line, const char *expr, nx_i8 val);
void nx_dbg_i16_(FILE *stream, const char *file, int line, const char *expr, nx_i16 val);
void nx_dbg_i32_(FILE *stream, const char *file, int line, const char *expr, nx_i32 val);
void nx_dbg_i64_(FILE *stream, const char *file, int line, const char *expr, nx_i64 val);
void nx_dbg_u8_(FILE *stream, const char *file, int line, const char *expr, nx_u8 val);
void nx_dbg_u16_(FILE *stream, const char *file, int line, const char *expr, nx_u16 val);
void nx_dbg_u32_(FILE *stream, const char *file, int line, const char *expr, nx_u32 val);
void nx_dbg_u64_(FILE *stream, const char *file, int line, const char *expr, nx_u64 val);
void nx_dbg_f32_(FILE *stream, const char *file, int line, const char *expr, nx_f32 val);
void nx_dbg_f64_(FILE *stream, const char *file, int line, const char *expr, nx_f64 val);
void nx_dbg_bool_(FILE *stream, const char *file, int line, const char *expr, nx_bool val);
void nx_dbg_cstr_(FILE *stream, const char *file, int line, const char *expr, const nx_char *val);
