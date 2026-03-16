#pragma once

#include "nx/core/fn.h"

/* ========== ascending order ========== */

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

int nx_cmp_f32(const void *lhs, const void *rhs);
int nx_cmp_f64(const void *lhs, const void *rhs);

int nx_cmp_cstr(const void *lhs, const void *rhs);

/* ========== descending order ========== */

int nx_cmp_desc_i8(const void *lhs, const void *rhs);
int nx_cmp_desc_i16(const void *lhs, const void *rhs);
int nx_cmp_desc_i32(const void *lhs, const void *rhs);
int nx_cmp_desc_i64(const void *lhs, const void *rhs);

int nx_cmp_desc_u8(const void *lhs, const void *rhs);
int nx_cmp_desc_u16(const void *lhs, const void *rhs);
int nx_cmp_desc_u32(const void *lhs, const void *rhs);
int nx_cmp_desc_u64(const void *lhs, const void *rhs);

int nx_cmp_desc_usize(const void *lhs, const void *rhs);
int nx_cmp_desc_isize(const void *lhs, const void *rhs);

int nx_cmp_desc_bool(const void *lhs, const void *rhs);
int nx_cmp_desc_byte(const void *lhs, const void *rhs);
int nx_cmp_desc_char(const void *lhs, const void *rhs);

int nx_cmp_desc_f32(const void *lhs, const void *rhs);
int nx_cmp_desc_f64(const void *lhs, const void *rhs);

int nx_cmp_desc_cstr(const void *lhs, const void *rhs);

/* ========== nx_eq_fn-compatible equality callbacks ========== */

nx_bool nx_eq_fn_i8(const void *a, const void *b);
nx_bool nx_eq_fn_i16(const void *a, const void *b);
nx_bool nx_eq_fn_i32(const void *a, const void *b);
nx_bool nx_eq_fn_i64(const void *a, const void *b);

nx_bool nx_eq_fn_u8(const void *a, const void *b);
nx_bool nx_eq_fn_u16(const void *a, const void *b);
nx_bool nx_eq_fn_u32(const void *a, const void *b);
nx_bool nx_eq_fn_u64(const void *a, const void *b);

nx_bool nx_eq_fn_usize(const void *a, const void *b);
nx_bool nx_eq_fn_isize(const void *a, const void *b);

nx_bool nx_eq_fn_bool(const void *a, const void *b);
nx_bool nx_eq_fn_byte(const void *a, const void *b);
nx_bool nx_eq_fn_char(const void *a, const void *b);

nx_bool nx_eq_fn_f32(const void *a, const void *b);
nx_bool nx_eq_fn_f64(const void *a, const void *b);

/// Compares through pointer indirection: a and b point to `const char *`.
nx_bool nx_eq_fn_cstr(const void *a, const void *b);
