#pragma once

// TODO: add generics

// TODO: use this instead of int (not sure)
typedef enum {
    NX_ORDERING_LESS = -1,
    NX_ORDERING_EQUAL = 0,
    NX_ORDERING_GREATER = 1,
} nx_Ordering;

typedef int (*nx_cmp_fn)(const void *, const void *);

// TODO: not used
typedef int (*nx_cmp_ctx_fn)(const void *, const void *, void *);

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
