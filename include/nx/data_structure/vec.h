#pragma once

#include <stddef.h>
#include <stdbool.h>

#include "nx/core/assert.h"
#include "nx/core/status.h"
#include "nx/core/span.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Contract:
 * - All invalid usage (NULL, bounds, invariants, elem_size mismatch, etc.) is a programmer error:
 *   guarded by NX_ASSERT (debug). Release behavior is unspecified.
 * - Status-returning functions report only internal failures:
 *   OUT_OF_MEMORY and MUL_OVERFLOW.
 */

typedef struct nx_vec nx_vec;

/* ---------- make/drop ---------- */

nx_status nx_vec_make(nx_vec **out, size_t len, size_t elem_size);
nx_status nx_vec_make_cap(nx_vec **out, size_t cap, size_t elem_size);
void nx_vec_drop(nx_vec *self);

/* ---------- copy/move semantic ---------- */

nx_status nx_vec_copy(nx_vec **out, const nx_vec *src);
nx_status nx_vec_move(nx_vec **out, nx_vec *src);
nx_status nx_vec_copy_assign(nx_vec *self, const nx_vec *src);
nx_status nx_vec_move_assign(nx_vec *self, nx_vec *src);

/* ---------- info ---------- */

size_t nx_vec_len(const nx_vec *self);
bool nx_vec_empty(const nx_vec *self);
size_t nx_vec_cap(const nx_vec *self);
size_t nx_vec_elem_size(const nx_vec *self);

/* ---------- access ---------- */

void *nx_vec_get(nx_vec *self, size_t idx);
const void *nx_vec_get_const(const nx_vec *self, size_t idx);
void nx_vec_set(nx_vec *self, size_t idx, const void *elem);
void *nx_vec_data(nx_vec *self);
const void *nx_vec_data_const(const nx_vec *self);

/* ---------- mods ---------- */

nx_status nx_vec_push(nx_vec *self, const void *elem);
void *nx_vec_pop(nx_vec *self);
void nx_vec_clear(nx_vec *self);
nx_status nx_vec_reserve(nx_vec *self, size_t new_cap);
nx_status nx_vec_resize(nx_vec *self, size_t new_len);
nx_status nx_vec_shrink_to_fit(nx_vec *self);
void nx_vec_swap(nx_vec *a, nx_vec *b);
nx_status nx_vec_insert(nx_vec *self, size_t idx, const void *elem);
void nx_vec_erase(nx_vec *self, size_t idx);

/* ---------- to span ---------- */

nx_span nx_vec_to_span(nx_vec *self);
nx_cspan nx_vec_to_cspan(const nx_vec *self);

/* ---------- macros ---------- */

#define NX_VEC_MAKE(out, T, len)    \
    nx_vec_make((out), (len), sizeof(T))

#define NX_VEC_MAKE_CAP(out, T, cap)    \
    nx_vec_make_cap((out), (cap), sizeof(T))

#define NX_VEC_GET_AS(T, self, idx)                       \
    (NX_ASSERT(nx_vec_elem_size((self)) == sizeof(T)),    \
    (T *) nx_vec_get((self), (idx)))

#define NX_VEC_GET_AS_C(T, self, idx)                     \
    (NX_ASSERT(nx_vec_elem_size((self)) == sizeof(T)),    \
    (const T *) nx_vec_get_const((self), (idx)))

#define NX_VEC_SET(T, self, idx, expr)                       \
    do {                                                     \
        NX_ASSERT(nx_vec_elem_size((self)) == sizeof(T));    \
        const T nx_tmp_ = (expr);                            \
        nx_vec_set((self), (idx), &nx_tmp_);                 \
    } while (0)

#define NX_VEC_DATA_AS(T, self)                           \
    (NX_ASSERT(nx_vec_elem_size((self)) == sizeof(T)),    \
    (T *) nx_vec_data((self)))

#define NX_VEC_DATA_AS_C(T, self)                          \
    (NX_ASSERT(nx_vec_elem_size((self)) == sizeof(T)),     \
    (const T *) nx_vec_data_const((self)))

/* ops that may allocate: return nx_status */

#define NX_VEC_PUSH_EXPR(T, self, expr)                            \
    do {                                                           \
        NX_ASSERT(nx_vec_elem_size((self)) == sizeof(T));          \
        const T nx_tmp_ = (expr);                                  \
        const nx_status nx_st_ = nx_vec_push((self), &nx_tmp_);    \
        NX_ASSERT(nx_st_ == NX_STATUS_OK);                         \
    } while (0)

#define NX_VEC_INSERT_EXPR(T, self, idx, expr)                              \
    do {                                                                    \
        NX_ASSERT(nx_vec_elem_size((self)) == sizeof(T));                   \
        const T nx_tmp_ = (expr);                                           \
        const nx_status nx_st_ = nx_vec_insert((self), (idx), &nx_tmp_);    \
        NX_ASSERT(nx_st_ == NX_STATUS_OK);                                  \
    } while (0)

/* pop returns pointer to last element storage (valid to read immediately) */
#define NX_VEC_POP_PTR(T, self)                           \
    (NX_ASSERT(nx_vec_elem_size((self)) == sizeof(T)),    \
    (T *) nx_vec_pop((self)))

#ifdef __cplusplus
}
#endif

