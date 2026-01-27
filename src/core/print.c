#include "nx/core/print.h"

#include <stdio.h>

#include "nx/core/assert.h"
#include "nx/core/panic.h"

#define NX_FPRINT_DEF(NAME, TYPE, FMT)                        \
    void nx_fprint_##NAME(FILE *stream, const void *data) {    \
        NX_ASSERT(stream);                                    \
        NX_ASSERT(data);                                      \
        fprintf(stream, FMT, *(const TYPE *) data);           \
    }

NX_FPRINT_DEF(c,   char,               "%c")
NX_FPRINT_DEF(uc,  unsigned char,      "%hhu")
NX_FPRINT_DEF(sc,  signed char,        "%hhd")

NX_FPRINT_DEF(s,   short,              "%hd")
NX_FPRINT_DEF(us,  unsigned short,     "%hu")

NX_FPRINT_DEF(i,   int,                "%d")
NX_FPRINT_DEF(ui,  unsigned int,       "%u")

NX_FPRINT_DEF(l,   long,               "%ld")
NX_FPRINT_DEF(ul,  unsigned long,      "%lu")

NX_FPRINT_DEF(ll,  long long,          "%lld")
NX_FPRINT_DEF(ull, unsigned long long, "%llu")

NX_FPRINT_DEF(f,   float,              "%f")
NX_FPRINT_DEF(d,   double,             "%lf")
NX_FPRINT_DEF(ld,  long double,        "%Lf")

void nx_fprint_span(FILE *stream, nx_span s, nx_fprint_func f) {
    nx_fprint_cspan(stream, nx_cspan_make_from_span(s), f);
}

void nx_fprint_cspan(FILE *stream, nx_cspan s, nx_fprint_func f) {
    NX_ASSERT(stream);
    NX_ANY_SPAN_ASSERT(s);
    NX_ASSERT(f);

    fputc('[', stream);
    for (size_t i = 0; i < s.len; ++i) {
        f(stream, nx_cspan_get_c(s, i));
        if (i + 1 != s.len) {
            fputs(", ", stream);
        }
    }
    fputs("]\n", stream);
}

