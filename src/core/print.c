#include "nx/core/print.h"

#include <stdarg.h>

#include "nx/core/assert.h"

/* ========== printf wrappers ========== */

nx_i32 nx_printf(const char *fmt, ...) {
    NX_ASSERT(fmt);

    va_list vl;
    va_start(vl, fmt);
    const nx_i32 n = vfprintf(stdout, fmt, vl);
    va_end(vl);
    return n;
}

nx_i32 nx_fprintf(FILE *stream, const char *fmt, ...) {
    NX_ASSERT(stream);
    NX_ASSERT(fmt);

    va_list vl;
    va_start(vl, fmt);
    const nx_i32 n = vfprintf(stream, fmt, vl);
    va_end(vl);
    return n;
}

/* ========== typed printers ========== */

#define NX_FPRINT_DEF(NAME, TYPE, FMT)                         \
    void nx_fprint_##NAME(FILE *stream, const void *data) {    \
        NX_ASSERT(stream);                                     \
        NX_ASSERT(data);                                       \
        fprintf(stream, FMT, *(const TYPE *) data);            \
    }

NX_FPRINT_DEF(i8,     nx_i8,     NX_FMT_I8)
NX_FPRINT_DEF(i16,    nx_i16,    NX_FMT_I16)
NX_FPRINT_DEF(i32,    nx_i32,    NX_FMT_I32)
NX_FPRINT_DEF(i64,    nx_i64,    NX_FMT_I64)

NX_FPRINT_DEF(u8,     nx_u8,     NX_FMT_U8)
NX_FPRINT_DEF(u16,    nx_u16,    NX_FMT_U16)
NX_FPRINT_DEF(u32,    nx_u32,    NX_FMT_U32)
NX_FPRINT_DEF(u64,    nx_u64,    NX_FMT_U64)

NX_FPRINT_DEF(usize,  nx_usize,  NX_FMT_USIZE)
NX_FPRINT_DEF(isize,  nx_isize,  NX_FMT_ISIZE)

NX_FPRINT_DEF(byte,   nx_byte,   NX_FMT_BYTE)
NX_FPRINT_DEF(char,   nx_char,   NX_FMT_CHAR)

NX_FPRINT_DEF(f32,    nx_f32,    NX_FMT_F32)
NX_FPRINT_DEF(f64,    nx_f64,    NX_FMT_F64)

void nx_fprint_bool(FILE *stream, const void *data) {
    NX_ASSERT(stream);
    NX_ASSERT(data);
    fputs(*(const nx_bool *) data ? "true" : "false", stream);
}

void nx_fprint_ptr(FILE *stream, const void *data) {
    NX_ASSERT(stream);
    NX_ASSERT(data);
    fprintf(stream, NX_FMT_PTR, *(void *const *) data);
}

/* ========== span printing ========== */

void nx_fprintln_cspan(FILE *stream, nx_cspan s, nx_fprint_fn f) {
    NX_ASSERT(stream);
    NX_SPAN_ANY_ASSERT(s);
    NX_ASSERT(f);

    fputc('[', stream);
    for (nx_usize i = 0; i < s.len; ++i) {
        if (i > 0) {
            fputs(", ", stream);
        }
        f(stream, nx_cspan_get_c(s, i));
    }
    fputs("]\n", stream);
}

void nx_fprintln_span(FILE *stream, nx_span s, nx_fprint_fn f) {
    nx_fprintln_cspan(stream, nx_cspan_from_span(s), f);
}

void nx_println_cspan(nx_cspan s, nx_fprint_fn f) {
    nx_fprintln_cspan(stdout, s, f);
}

void nx_println_span(nx_span s, nx_fprint_fn f) {
    nx_fprintln_span(stdout, s, f);
}