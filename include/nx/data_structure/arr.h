#pragma once

#include "nx/core/type.h"
#include "nx/core/assert.h"
#include "nx/core/status.h"
#include "nx/core/span.h"
#include "nx/core/result.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Contract:
 * - All invalid usage (NULL, bounds, invariants, elem_size mismatch, etc.) is a programmer error:
 *   guarded by NX_ASSERT in debug builds. Release behavior is unspecified.
 *
 * - Functions that may fail due to internal reasons (allocation, overflow) report failures via:
 *   - nx_status (for mutating operations), or
 *   - nx_*_res (Result-like return) for constructors/factories.
 *
 * - Result convention:
 *   - st == NX_STATUS_OK => val is valid.
 *   - st != NX_STATUS_OK => val must not be used (NX_RES_GET_VAL asserts).
 */

typedef struct nx_arr nx_arr;

NX_DEF_RESULT_TYPE(nx_arr_res, nx_arr *);

/* ---------- lifetime ---------- */

nx_arr_res nx_arr_new(nx_usize len, nx_usize elem_size);
void nx_arr_drop(nx_arr *self);

/* ---------- copy/move semantic ---------- */

nx_arr_res nx_arr_copy(const nx_arr *src);
nx_arr *nx_arr_move(nx_arr **src);
nx_status nx_arr_copy_assign(nx_arr *self, const nx_arr *src);
void nx_arr_move_assign(nx_arr *self, nx_arr *src);

/* ---------- info ---------- */

nx_usize nx_arr_len(const nx_arr *self);
nx_bool nx_arr_empty(const nx_arr *self);
nx_usize nx_arr_elem_size(const nx_arr *self);

/* ---------- access ---------- */

void *nx_arr_get(nx_arr *self, nx_usize idx);
const void *nx_arr_get_c(const nx_arr *self, nx_usize idx);
void nx_arr_set(nx_arr *self, nx_usize idx, const void *elem);
void *nx_arr_data(nx_arr *self);
const void *nx_arr_data_c(const nx_arr *self);

/* ---------- mods ---------- */

void nx_arr_swap(nx_arr *a, nx_arr *b);

/* ---------- to span ---------- */

nx_span nx_arr_to_span(nx_arr *self);
nx_cspan nx_arr_to_cspan(const nx_arr *self);

/* ---------- macros ---------- */

#define NX_ARR_NEW_LEN(T, len)    \
    nx_arr_new((len), sizeof(T))

#define NX_ARR_GET_AS(T, self, idx)                       \
    (NX_ASSERT(nx_arr_elem_size((self)) == sizeof(T)),    \
    (T *) nx_arr_get((self), (idx)))

#define NX_ARR_GET_AS_C(T, self, idx)                     \
    (NX_ASSERT(nx_arr_elem_size((self)) == sizeof(T)),    \
    (const T *) nx_arr_get_c((self), (idx)))

#define NX_ARR_SET_EXPR(T, self, idx, expr)                  \
    do {                                                     \
        NX_ASSERT(nx_arr_elem_size((self)) == sizeof(T));    \
        const T nx_tmp_ = (expr);                            \
        nx_arr_set((self), (idx), &nx_tmp_);                 \
    } while (0)

#define NX_ARR_DATA_AS(T, self)                           \
    (NX_ASSERT(nx_arr_elem_size((self)) == sizeof(T)),    \
    (T *) nx_arr_data((self)))

#define NX_ARR_DATA_AS_C(T, self)                         \
    (NX_ASSERT(nx_arr_elem_size((self)) == sizeof(T)),    \
    (const T *) nx_arr_data_const((self)))

#ifdef __cplusplus
}
#endif
