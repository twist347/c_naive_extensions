#pragma once

#include <stdio.h>

#include "nx/core/span.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*nx_print_func)(FILE *out, const void *);

void nx_fprint_c(FILE *out, const void *data);
void nx_fprint_uc(FILE *out, const void *data);
void nx_fprint_sc(FILE *out, const void *data);

void nx_fprint_s(FILE *out, const void *data);
void nx_fprint_us(FILE *out, const void *data);

void nx_fprint_i(FILE *out, const void *data);
void nx_fprint_ui(FILE *out, const void *data);

void nx_fprint_l(FILE *out, const void *data);
void nx_fprint_ul(FILE *out, const void *data);

void nx_fprint_ll(FILE *out, const void *data);
void nx_fprint_ull(FILE *out, const void *data);

void nx_fprint_f(FILE *out, const void *data);
void nx_fprint_d(FILE *out, const void *data);
void nx_fprint_ld(FILE *out, const void *data);

void nx_fprint_cspan(FILE *out, nx_cspan s, nx_print_func f);

void nx_print_c(const void *data);
void nx_print_uc(const void *data);
void nx_print_sc(const void *data);

void nx_print_s(const void *data);
void nx_print_us(const void *data);

void nx_print_i(const void *data);
void nx_print_ui(const void *data);

void nx_print_l(const void *data);
void nx_print_ul(const void *data);

void nx_print_ll(const void *data);
void nx_print_ull(const void *data);

void nx_print_f(const void *data);
void nx_print_d(const void *data);
void nx_print_ld(const void *data);

void nx_print_cspan(nx_cspan s, nx_print_func f);

#ifdef __cplusplus
}
#endif

