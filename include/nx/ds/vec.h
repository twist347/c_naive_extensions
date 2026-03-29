#pragma once

#include "nx/core/type.h"
#include "nx/core/assert.h"
#include "nx/core/status.h"
#include "nx/core/result.h"
#include "nx/ds/span.h"
#include "nx/mem/alloc.h"

/* Contract:
 * - Invalid usage (null, bounds, invariants, type mismatch) is a programmer
 *   error guarded by NX_ASSERT. Release behavior is unspecified.
 *
 * - The only recoverable failure is allocation failure:
 *   - constructors return nx_*_res
 *   - mutating operations return nx_status
 *
 * - Result convention:
 *   - st == NX_STATUS_OK  => val is valid
 *   - st != NX_STATUS_OK  => val must not be used
 *
 * - Null data <--> zero length
 *
 * - Allocator:
 *   - NOT owned; must outlive the container
 *   - Simple API uses nx_al_libc_default_g()
 *   - For custom allocator, use nx_*_new_p with params
 *
 * - No overflow checking
 */

typedef struct nx_Vec nx_Vec;

NX_DEF_RES_TYPE(nx_VecRes, nx_Vec *);

/* ========== params ========== */

typedef struct {
    nx_usize len;
    nx_usize cap;
    nx_usize tsz; // type size
    nx_Al *al;    // must not be null
} nx_VecParams;

/* ========== lifetime ========== */

/// create with full params. zero-initialized data.
nx_VecRes nx_vec_new_p(nx_VecParams p);

/// empty vec, no allocation until first push.
nx_VecRes nx_vec_new(nx_usize tsz);

/// zero-initialized vec of given length (cap == len).
nx_VecRes nx_vec_new_len(nx_usize len, nx_usize tsz);

/// empty vec with pre-allocated capacity.
nx_VecRes nx_vec_new_cap(nx_usize cap, nx_usize tsz);

/// copies len elements from data.
nx_VecRes nx_vec_from_data(const void *data, nx_usize len, nx_usize tsz);
nx_VecRes nx_vec_from_span(nx_CSpan s);

/// frees memory. safe to call with null.
void nx_vec_drop(nx_Vec *self);

/* ========== copy/move semantic ========== */

/// deep copy with same allocator. returns new vec.
nx_VecRes nx_vec_copy(const nx_Vec *src);

/// deep copy using a different allocator.
nx_VecRes nx_vec_copy_a(const nx_Vec *src, nx_Al *al);

/// transfers ownership: returns *src, sets *src to null.
nx_Vec *nx_vec_move(nx_Vec **src);

/// deep copy into existing vec. reuses buffer if it fits.
nx_Status nx_vec_copy_assign(nx_Vec *self, const nx_Vec *src);

/// transfers src's data into self. src becomes empty. same allocator required.
void nx_vec_move_assign(nx_Vec *self, nx_Vec *src);

/* ========== info ========== */

nx_usize nx_vec_len(const nx_Vec *self);
nx_bool nx_vec_empty(const nx_Vec *self);
nx_usize nx_vec_cap(const nx_Vec *self);
nx_usize nx_vec_tsz(const nx_Vec *self);
nx_Al *nx_vec_al(const nx_Vec *self);

/* ========== access ========== */

/// get: unchecked in release (asserts in debug).
void *nx_vec_get(nx_Vec *self, nx_usize idx);
const void *nx_vec_get_c(const nx_Vec *self, nx_usize idx);

/// at: returns null if idx is out of bounds.
void *nx_vec_at(nx_Vec *self, nx_usize idx);
const void *nx_vec_at_c(const nx_Vec *self, nx_usize idx);

/// val must not point to an element within the same vector.
void nx_vec_set(nx_Vec *self, nx_usize idx, const void *val);

void *nx_vec_data(nx_Vec *self);
const void *nx_vec_data_c(const nx_Vec *self);

/* ========== mods ========== */

/// copies val into the end. may reallocate.
nx_Status nx_vec_push(nx_Vec *self, const void *val);

/// removes last element. returns pointer to it (valid until next push/insert).
void *nx_vec_pop(nx_Vec *self);

/// sets len to 0 without freeing memory.
void nx_vec_clear(nx_Vec *self);

/// ensures capacity >= new_cap. no-op if already sufficient.
nx_Status nx_vec_reserve(nx_Vec *self, nx_usize new_cap);

/// sets len to new_len. new elements are zero-initialized. may reallocate.
nx_Status nx_vec_resize(nx_Vec *self, nx_usize new_len);

/// reduces capacity to match len.
nx_Status nx_vec_shrink_to_fit(nx_Vec *self);

/// swaps contents of a and b (pointer swap, O(1)). tsz must match.
void nx_vec_swap(nx_Vec *a, nx_Vec *b);

/// inserts val at idx, shifting elements right. idx must be <= len.
nx_Status nx_vec_insert(nx_Vec *self, nx_usize idx, const void *val);

/// removes element at idx, shifting elements left.
void nx_vec_erase(nx_Vec *self, nx_usize idx);
/// append all elements from span. tsz must match.
nx_Status nx_vec_extend(nx_Vec *self, nx_CSpan s);

/* ========== to span ========== */

nx_Span nx_vec_to_span(nx_Vec *self);
nx_CSpan nx_vec_to_cspan(const nx_Vec *self);

/* ========== print ========== */

void nx_vec_fprintln(const nx_Vec *self, FILE *stream, nx_fprint_fn f);
void nx_vec_println(const nx_Vec *self, nx_fprint_fn f);

/* ========== macros ========== */

#define NX_VEC_NEW(T) \
    nx_vec_new(sizeof(T))

#define NX_VEC_NEW_LEN(T, len) \
    nx_vec_new_len((len), sizeof(T))

#define NX_VEC_NEW_CAP(T, cap) \
    nx_vec_new_cap((cap), sizeof(T))

#define NX_VEC_FROM_DATA(T, data, len) \
    nx_vec_from_data((data), (len), sizeof(T))

#define NX_VEC_GET_AS(T, self, idx)              \
    (NX_ASSERT(nx_vec_tsz((self)) == sizeof(T)), \
    (T *) nx_vec_get((self), (idx)))

#define NX_VEC_GET_AS_C(T, self, idx)            \
    (NX_ASSERT(nx_vec_tsz((self)) == sizeof(T)), \
    (const T *) nx_vec_get_c((self), (idx)))

#define NX_VEC_AT_AS(T, self, idx)               \
    (NX_ASSERT(nx_vec_tsz((self)) == sizeof(T)), \
    (T *) nx_vec_at((self), (idx)))

#define NX_VEC_AT_AS_C(T, self, idx)             \
    (NX_ASSERT(nx_vec_tsz((self)) == sizeof(T)), \
    (const T *) nx_vec_at_c((self), (idx)))

#define NX_VEC_SET(T, self, idx, expr)              \
    do {                                            \
        NX_ASSERT(nx_vec_tsz((self)) == sizeof(T)); \
        const T nx_tmp_ = (expr);                   \
        nx_vec_set((self), (idx), &nx_tmp_);        \
    } while (0)

#define NX_VEC_DATA_AS(T, self)                  \
    (NX_ASSERT(nx_vec_tsz((self)) == sizeof(T)), \
    (T *) nx_vec_data((self)))

#define NX_VEC_DATA_AS_C(T, self)                \
    (NX_ASSERT(nx_vec_tsz((self)) == sizeof(T)), \
    (const T *) nx_vec_data_c((self)))

/* ops that may allocate: return nx_status */

#define NX_VEC_PUSH_EXPR(T, self, expr)                         \
    do {                                                        \
        NX_ASSERT(nx_vec_tsz((self)) == sizeof(T));             \
        const T nx_tmp_ = (expr);                               \
        const nx_Status nx_st_ = nx_vec_push((self), &nx_tmp_); \
        NX_ASSERT(nx_st_ == NX_STATUS_OK);                      \
    } while (0)

#define NX_VEC_INSERT_EXPR(T, self, idx, expr)                           \
    do {                                                                 \
        NX_ASSERT(nx_vec_tsz((self)) == sizeof(T));                      \
        const T nx_tmp_ = (expr);                                        \
        const nx_Status nx_st_ = nx_vec_insert((self), (idx), &nx_tmp_); \
        NX_ASSERT(nx_st_ == NX_STATUS_OK);                               \
    } while (0)

/* pop returns pointer to last element storage (valid to read immediately) */
#define NX_VEC_POP_PTR(T, self)                  \
    (NX_ASSERT(nx_vec_tsz((self)) == sizeof(T)), \
    (T *) nx_vec_pop((self)))
