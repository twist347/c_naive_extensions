#include "nx/core/print.h"

#include <inttypes.h>

#include "nx/core/assert.h"
#include "nx/core/panic.h"
#include "nx/core/type.h"

#define NX_FPRINT_DEF(NAME, TYPE, FMT)                         \
    void nx_fprint_##NAME(FILE *stream, const void *data) {    \
        NX_ASSERT(stream);                                     \
        NX_ASSERT(data);                                       \
        fprintf(stream, FMT, *(const TYPE *) data);            \
    }

NX_FPRINT_DEF(i8,     nx_i8,     "%" PRId8)
NX_FPRINT_DEF(i16,    nx_i16,    "%" PRId16)
NX_FPRINT_DEF(i32,    nx_i32,    "%" PRId32)
NX_FPRINT_DEF(i64,    nx_i64,    "%" PRId64)

NX_FPRINT_DEF(u8,     nx_u8,     "%" PRIu8)
NX_FPRINT_DEF(u16,    nx_u16,    "%" PRIu16)
NX_FPRINT_DEF(u32,    nx_u32,    "%" PRIu32)
NX_FPRINT_DEF(u64,    nx_u64,    "%" PRIu64)

NX_FPRINT_DEF(usize,  nx_usize,  "%zu")
NX_FPRINT_DEF(isize,  nx_isize,  "%td")

void nx_fprint_bool(FILE *stream, const void *data) {
    NX_ASSERT(stream);
    NX_ASSERT(data);
    fputs(*(const nx_bool *) data ? "true" : "false", stream);
}

NX_FPRINT_DEF(byte,   nx_byte,   "%u")
NX_FPRINT_DEF(char,   nx_char,   "%c")

NX_FPRINT_DEF(flt,    nx_flt,    "%g")
NX_FPRINT_DEF(dbl,    nx_dbl,    "%g")

NX_FPRINT_DEF(f32,    nx_f32,    "%g")
NX_FPRINT_DEF(f64,    nx_f64,    "%g")

void nx_fprintln_span(FILE *stream, nx_span s, nx_fprint_func f) {
    nx_fprintln_cspan(stream, nx_cspan_from_span(s), f);
}

void nx_fprintln_cspan(FILE *stream, nx_cspan s, nx_fprint_func f) {
    NX_ASSERT(stream);
    NX_ANY_SPAN_ASSERT(s);
    NX_ASSERT(f);

    fputc('[', stream);
    for (nx_usize i = 0; i < s.len; ++i) {
        f(stream, nx_cspan_get_c(s, i));
        if (i + 1 != s.len) {
            fputs(", ", stream);
        }
    }
    fputs("]\n", stream);
}

