#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*nx_cmp)(const void *, const void *);
typedef int (*nx_cmp_ctx)(const void *, const void *, void *);

int nx_cmp_c(const void *lhs, const void *rhs);
int nx_cmp_sc(const void *lhs, const void *rhs);
int nx_cmp_uc(const void *lhs, const void *rhs);

int nx_cmp_s(const void *lhs, const void *rhs);
int nx_cmp_us(const void *lhs, const void *rhs);

int nx_cmp_i(const void *lhs, const void *rhs);
int nx_cmp_ui(const void *lhs, const void *rhs);

int nx_cmp_l(const void *lhs, const void *rhs);
int nx_cmp_ul(const void *lhs, const void *rhs);

int nx_cmp_ll(const void *lhs, const void *rhs);
int nx_cmp_ull(const void *lhs, const void *rhs);

int nx_cmp_f(const void *lhs, const void *rhs);
int nx_cmp_d(const void *lhs, const void *rhs);
int nx_cmp_ld(const void *lhs, const void *rhs);

int nx_cmp_zu(const void *lhs, const void *rhs);
int nx_cmp_cstr(const void *lhs, const void *rhs);

#ifdef __cplusplus
}
#endif
