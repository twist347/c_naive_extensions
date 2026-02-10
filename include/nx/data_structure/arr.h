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
 * - All invalid usage (NULL, bounds, invariants, type_size mismatch, size overflow, etc.)
 *   is a programmer error guarded by NX_ASSERT in debug builds. Release behavior is unspecified.
 *
 * - The only recoverable failures are allocation failures:
 *   - mutating operations return nx_status,
 *   - constructors/factories return nx_*_res.
 *
 * - Result convention:
 *   - st == NX_STATUS_OK => val is valid.
 *   - st != NX_STATUS_OK => val must not be used.
 */

typedef struct nx_arr nx_arr;

NX_DEF_RESULT_TYPE(nx_arr_res, nx_arr *);

/* ========== params ========== */

typedef struct nx_arr_params {
    nx_usize len;
    nx_usize tsz; // type size
} nx_arr_params;

/* ========== lifetime ========== */

nx_arr_res nx_arr_new_p(nx_arr_params p);
nx_arr_res nx_arr_new_len(nx_usize len, nx_usize tsz);
void nx_arr_drop(nx_arr *self);

/* ========== copy/move semantic ========== */

nx_arr_res nx_arr_copy(const nx_arr *src);
nx_arr *nx_arr_move(nx_arr **src);
nx_status nx_arr_copy_assign(nx_arr *self, const nx_arr *src);
void nx_arr_move_assign(nx_arr *self, nx_arr *src);

/* ========== info ========== */

nx_usize nx_arr_len(const nx_arr *self);
nx_bool nx_arr_empty(const nx_arr *self);
nx_usize nx_arr_tsz(const nx_arr *self);

/* ========== access ========== */

void *nx_arr_get(nx_arr *self, nx_usize idx);
const void *nx_arr_get_c(const nx_arr *self, nx_usize idx);
void *nx_arr_at(nx_arr *self, nx_usize idx);
const void *nx_arr_at_c(const nx_arr *self, nx_usize idx);
void nx_arr_set(nx_arr *self, nx_usize idx, const void *val);
void *nx_arr_data(nx_arr *self);
const void *nx_arr_data_c(const nx_arr *self);

/* ========== mods ========== */

void nx_arr_swap(nx_arr *a, nx_arr *b);

/* ========== to span ========== */

nx_span nx_arr_to_span(nx_arr *self);
nx_cspan nx_arr_to_cspan(const nx_arr *self);

/* ========== macros ========== */

#define NX_ARR_NEW_LEN(T, len)    \
    nx_arr_new_len((len), sizeof(T))

#define NX_ARR_GET_AS(T, self, idx)                 \
    (NX_ASSERT(nx_arr_tsz((self)) == sizeof(T)),    \
    (T *) nx_arr_get((self), (idx)))

#define NX_ARR_GET_AS_C(T, self, idx)               \
    (NX_ASSERT(nx_arr_tsz((self)) == sizeof(T)),    \
    (const T *) nx_arr_get_c((self), (idx)))

#define NX_ARR_AT_AS(T, self, idx)                  \
    (NX_ASSERT(nx_arr_tsz((self)) == sizeof(T)),    \
    (T *) nx_arr_at((self), (idx)))

#define NX_ARR_AT_AS_C(T, self, idx)                \
    (NX_ASSERT(nx_arr_tsz((self)) == sizeof(T)),    \
    (const T *) nx_arr_at_c((self), (idx)))

#define NX_ARR_SET_EXPR(T, self, idx, expr)            \
    do {                                               \
        NX_ASSERT(nx_arr_tsz((self)) == sizeof(T));    \
        const T nx_tmp_ = (expr);                      \
        nx_arr_set((self), (idx), &nx_tmp_);           \
    } while (0)

#define NX_ARR_DATA_AS(T, self)                     \
    (NX_ASSERT(nx_arr_tsz((self)) == sizeof(T)),    \
    (T *) nx_arr_data((self)))

#define NX_ARR_DATA_AS_C(T, self)                   \
    (NX_ASSERT(nx_arr_tsz((self)) == sizeof(T)),    \
    (const T *) nx_arr_data_c((self)))

#ifdef __cplusplus
}
#endif
