#pragma once

#include <stdio.h>

#include "nx/core/span.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*nx_fprint_func)(FILE *, const void *);

void nx_fprint_c(FILE *stream, const void *data);
void nx_fprint_uc(FILE *stream, const void *data);
void nx_fprint_sc(FILE *stream, const void *data);

void nx_fprint_s(FILE *stream, const void *data);
void nx_fprint_us(FILE *stream, const void *data);

void nx_fprint_i(FILE *stream, const void *data);
void nx_fprint_ui(FILE *stream, const void *data);

void nx_fprint_l(FILE *stream, const void *data);
void nx_fprint_ul(FILE *stream, const void *data);

void nx_fprint_ll(FILE *stream, const void *data);
void nx_fprint_ull(FILE *stream, const void *data);

void nx_fprint_f(FILE *stream, const void *data);
void nx_fprint_d(FILE *stream, const void *data);
void nx_fprint_ld(FILE *stream, const void *data);

void nx_fprint_span(FILE *stream, nx_span s, nx_fprint_func f);
void nx_fprint_cspan(FILE *stream, nx_cspan s, nx_fprint_func f);

#ifdef __cplusplus
}
#endif

