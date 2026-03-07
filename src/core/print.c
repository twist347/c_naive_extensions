#include "nx/core/print.h"

#include <stdarg.h>

#include "nx/core/assert.h"

/* ========== formated print ========== */

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

/* ========== print ========== */

nx_i32 fprint(FILE *stream, const char *str) {
    return fputs(str ? str : "null", stream);
}

nx_i32 nx_fprintln(FILE *stream, const char *str) {
    NX_ASSERT(stream);
    const nx_i32 n = fputs(str ? str : "null", stream);
    if (n >= 0) {
        fputc('\n', stream);
    }
    return n;
}

nx_i32 nx_print(const char *str) {
    return fprint(stdout, str);
}

nx_i32 nx_println(const char *str) {
    return nx_fprintln(stdout, str);
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

NX_FPRINT_DEF(f32,    nx_f32,    NX_FMT_F32)
NX_FPRINT_DEF(f64,    nx_f64,    NX_FMT_F64)

NX_FPRINT_DEF(usize,  nx_usize,  NX_FMT_USIZE)
NX_FPRINT_DEF(isize,  nx_isize,  NX_FMT_ISIZE)

NX_FPRINT_DEF(byte,   nx_byte,   NX_FMT_BYTE)
NX_FPRINT_DEF(char,   nx_char,   NX_FMT_CHAR)

void nx_fprint_bool(FILE *stream, const void *data) {
    NX_ASSERT(stream);
    NX_ASSERT(data);
    fputs(*(const nx_bool *) data ? "true" : "false", stream);
}

void nx_fprint_str(FILE *stream, const void *data) {
    NX_ASSERT(stream);
    NX_ASSERT(data);
    const char *s = *(const char *const *) data;
    fputs(s ? s : "(null)", stream);
}

void nx_fprint_ptr(FILE *stream, const void *data) {
    NX_ASSERT(stream);
    NX_ASSERT(data);
    fprintf(stream, NX_FMT_PTR, *(void *const *) data);
}
