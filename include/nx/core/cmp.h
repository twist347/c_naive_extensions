#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*nx_cmp)(const void *, const void *);

// TODO: not used
typedef int (*nx_cmp_ctx)(const void *, const void *, void *);

int nx_cmp_i8(const void *lhs, const void *rhs);
int nx_cmp_i16(const void *lhs, const void *rhs);
int nx_cmp_i32(const void *lhs, const void *rhs);
int nx_cmp_i64(const void *lhs, const void *rhs);

int nx_cmp_u8(const void *lhs, const void *rhs);
int nx_cmp_u16(const void *lhs, const void *rhs);
int nx_cmp_u32(const void *lhs, const void *rhs);
int nx_cmp_u64(const void *lhs, const void *rhs);

int nx_cmp_usize(const void *lhs, const void *rhs);
int nx_cmp_isize(const void *lhs, const void *rhs);

int nx_cmp_bool(const void *lhs, const void *rhs);
int nx_cmp_byte(const void *lhs, const void *rhs);
int nx_cmp_char(const void *lhs, const void *rhs);

int nx_cmp_flt(const void *lhs, const void *rhs);
int nx_cmp_dbl(const void *lhs, const void *rhs);

int nx_cmp_f32(const void *lhs, const void *rhs);
int nx_cmp_f64(const void *lhs, const void *rhs);

int nx_cmp_cstr(const void *lhs, const void *rhs);

#ifdef __cplusplus
}
#endif
