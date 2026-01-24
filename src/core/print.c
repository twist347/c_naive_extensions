#include "nx/core/print.h"

#include <stdio.h>

#include "nx/core/assert.h"

#define NX_FPRINT_DEF(NAME, TYPE, FMT)                      \
    void nx_fprint_##NAME(FILE *out, const void *data) {    \
        NX_ASSERT(out);                                     \
        NX_ASSERT(data);                                    \
        fprintf(out, FMT, *(const TYPE *) data);            \
    }

#define NX_PRINT_WRAP(NAME)                     \
    void nx_print_##NAME(const void *data) {    \
        nx_fprint_##NAME(stdout, data);         \
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

void nx_fprint_cspan(FILE *out, nx_cspan s, nx_print_func f) {
    NX_ASSERT(out);
    NX_SPAN_ASSERT(s);
    NX_ASSERT(f);

    fputc('[', out);
    for (size_t i = 0; i < s.len; ++i) {
        f(out, nx_cspan_get(s, i));
        if (i + 1 != s.len) {
            fputs(", ", out);
        }
    }
    fputs("]\n", out);
}

NX_PRINT_WRAP(c)
NX_PRINT_WRAP(uc)
NX_PRINT_WRAP(sc)

NX_PRINT_WRAP(s)
NX_PRINT_WRAP(us)

NX_PRINT_WRAP(i)
NX_PRINT_WRAP(ui)

NX_PRINT_WRAP(l)
NX_PRINT_WRAP(ul)

NX_PRINT_WRAP(ll)
NX_PRINT_WRAP(ull)

NX_PRINT_WRAP(f)
NX_PRINT_WRAP(d)
NX_PRINT_WRAP(ld)

void nx_print_cspan(nx_cspan s, nx_print_func f) {
    nx_fprint_cspan(stdout, s, f);
}
