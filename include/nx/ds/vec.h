#pragma once

#include "nx/core/type.h"
#include "nx/core/assert.h"
#include "nx/core/status.h"
#include "nx/ds/span.h"
#include "nx/core/result.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Contract:
 * - All invalid usage (NULL, bounds, invariants, type_size mismatch, size overflow, etc.)
 *   is a programmer error guarded by NX_ASSERT in debug builds. Release behavior is unspecified.
 *
 * - The only recoverable failures are allocation failures:
 *   - operations that may allocate return nx_status,
 *   - constructors/factories return nx_vec_res.
 *
 * - Result convention:
 *   - st == NX_STATUS_OK => val is valid.
 *   - st != NX_STATUS_OK => val must not be used.
 *
 * - zero-length <-> null data
 */

typedef struct nx_vec nx_vec;

NX_DEF_RES_TYPE(nx_vec_res, nx_vec *);

/* ========== params ========== */

typedef struct nx_vec_params {
    nx_usize len;
    nx_usize cap;
    nx_usize tsz; // type size
} nx_vec_params;

/* ========== lifetime ========== */

nx_vec_res nx_vec_new_p(nx_vec_params p);
nx_vec_res nx_vec_new(nx_usize tsz);
nx_vec_res nx_vec_new_len(nx_usize len, nx_usize tsz);
nx_vec_res nx_vec_new_cap(nx_usize cap, nx_usize tsz);
nx_vec_res nx_vec_from_data(const void *data, nx_usize len, nx_usize tsz);
void nx_vec_drop(nx_vec *self);

/* ========== copy/move semantic ========== */

nx_vec_res nx_vec_copy(const nx_vec *src);
nx_vec *nx_vec_move(nx_vec **src);
nx_status nx_vec_copy_assign(nx_vec *self, const nx_vec *src);
void nx_vec_move_assign(nx_vec *self, nx_vec *src);

/* ========== info ========== */

nx_usize nx_vec_len(const nx_vec *self);
nx_bool nx_vec_empty(const nx_vec *self);
nx_usize nx_vec_cap(const nx_vec *self);
nx_usize nx_vec_tsz(const nx_vec *self);

/* ========== access ========== */

void *nx_vec_get(nx_vec *self, nx_usize idx);
const void *nx_vec_get_c(const nx_vec *self, nx_usize idx);
void *nx_vec_at(nx_vec *self, nx_usize idx);
const void *nx_vec_at_c(const nx_vec *self, nx_usize idx);

/// val must not point to an element within the same vector.
void nx_vec_set(nx_vec *self, nx_usize idx, const void *val);

void *nx_vec_data(nx_vec *self);
const void *nx_vec_data_c(const nx_vec *self);

/* ========== mods ========== */

nx_status nx_vec_push(nx_vec *self, const void *val);
void *nx_vec_pop(nx_vec *self);
void nx_vec_clear(nx_vec *self);
nx_status nx_vec_reserve(nx_vec *self, nx_usize new_cap);
nx_status nx_vec_resize(nx_vec *self, nx_usize new_len);
nx_status nx_vec_shrink_to_fit(nx_vec *self);
void nx_vec_swap(nx_vec *a, nx_vec *b);
nx_status nx_vec_insert(nx_vec *self, nx_usize idx, const void *val);
void nx_vec_erase(nx_vec *self, nx_usize idx);

/* ========== to span ========== */

nx_span nx_vec_to_span(nx_vec *self);
nx_cspan nx_vec_to_cspan(const nx_vec *self);

/* ========== macros ========== */

#define NX_VEC_NEW(T) \
    nx_vec_new(sizeof(T))

#define NX_VEC_NEW_LEN(T, len) \
    nx_vec_new_len((len), sizeof(T))

#define NX_VEC_NEW_CAP(T, cap) \
    nx_vec_new_cap((cap), sizeof(T))

#define NX_VEC_FROM_DATA(T, data, len) \
    nx_vec_from_data((data), (len), sizeof(T))

#define NX_VEC_GET_AS(T, self, idx)                 \
    (NX_ASSERT(nx_vec_tsz((self)) == sizeof(T)),    \
    (T *) nx_vec_get((self), (idx)))

#define NX_VEC_GET_AS_C(T, self, idx)               \
    (NX_ASSERT(nx_vec_tsz((self)) == sizeof(T)),    \
    (const T *) nx_vec_get_c((self), (idx)))

#define NX_VEC_AT_AS(T, self, idx)                  \
    (NX_ASSERT(nx_vec_tsz((self)) == sizeof(T)),    \
    (T *) nx_vec_at((self), (idx)))

#define NX_VEC_AT_AS_C(T, self, idx)                \
    (NX_ASSERT(nx_vec_tsz((self)) == sizeof(T)),    \
    (const T *) nx_vec_at_c((self), (idx)))

#define NX_VEC_SET(T, self, idx, expr)                 \
    do {                                               \
        NX_ASSERT(nx_vec_tsz((self)) == sizeof(T));    \
        const T nx_tmp_ = (expr);                      \
        nx_vec_set((self), (idx), &nx_tmp_);           \
    } while (0)

#define NX_VEC_DATA_AS(T, self)                     \
    (NX_ASSERT(nx_vec_tsz((self)) == sizeof(T)),    \
    (T *) nx_vec_data((self)))

#define NX_VEC_DATA_AS_C(T, self)                   \
    (NX_ASSERT(nx_vec_tsz((self)) == sizeof(T)),    \
    (const T *) nx_vec_data_c((self)))

/* ops that may allocate: return nx_status */

#define NX_VEC_PUSH_EXPR(T, self, expr)                              \
    do {                                                             \
        NX_ASSERT(nx_vec_tsz((self)) == sizeof(T));                  \
        const T nx_tmp_ = (expr);                                    \
        NX_ASSERT(nx_vec_push((self), &nx_tmp_) == NX_STATUS_OK);    \
    } while (0)

#define NX_VEC_INSERT_EXPR(T, self, idx, expr)                              \
    do {                                                                    \
        NX_ASSERT(nx_vec_tsz((self)) == sizeof(T));                         \
        const T nx_tmp_ = (expr);                                           \
        const nx_status nx_st_ = nx_vec_insert((self), (idx), &nx_tmp_);    \
        NX_ASSERT(nx_st_ == NX_STATUS_OK);                                  \
    } while (0)

/* pop returns pointer to last element storage (valid to read immediately) */
#define NX_VEC_POP_PTR(T, self)                     \
    (NX_ASSERT(nx_vec_tsz((self)) == sizeof(T)),    \
    (T *) nx_vec_pop((self)))

#ifdef __cplusplus
}
#endif

