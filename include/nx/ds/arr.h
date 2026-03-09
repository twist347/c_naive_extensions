#pragma once

#include "nx/core/type.h"
#include "nx/core/assert.h"
#include "nx/core/status.h"
#include "nx/core/result.h"
#include "nx/ds/span.h"
#include "nx/mem/alloc.h"
#include "nx/io/print.h"

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

typedef struct nx_Arr nx_Arr;

NX_DEF_RES_TYPE(nx_ArrRes, nx_Arr *);

/* ========== params ========== */

typedef struct {
    nx_usize len;
    nx_usize tsz; // type size
    nx_Al *al;    // must not be null
} nx_ArrParams;

/* ========== lifetime ========== */

nx_ArrRes nx_arr_new_p(nx_ArrParams p);
nx_ArrRes nx_arr_new_len(nx_usize len, nx_usize tsz);
nx_ArrRes nx_arr_from_data(const void *data, nx_usize len, nx_usize tsz);
void nx_arr_drop(nx_Arr *self);

/* ========== copy/move semantic ========== */

nx_ArrRes nx_arr_copy(const nx_Arr *src);
nx_ArrRes nx_arr_copy_a(const nx_Arr *src, nx_Al *al);
nx_Arr *nx_arr_move(nx_Arr **src);
nx_Status nx_arr_copy_assign(nx_Arr *self, const nx_Arr *src);
void nx_arr_move_assign(nx_Arr *self, nx_Arr *src);

/* ========== info ========== */

nx_usize nx_arr_len(const nx_Arr *self);
nx_bool nx_arr_empty(const nx_Arr *self);
nx_usize nx_arr_tsz(const nx_Arr *self);
nx_Al *nx_arr_al(const nx_Arr *self);

/* ========== access ========== */

void *nx_arr_get(nx_Arr *self, nx_usize idx);
const void *nx_arr_get_c(const nx_Arr *self, nx_usize idx);
void *nx_arr_at(nx_Arr *self, nx_usize idx);
const void *nx_arr_at_c(const nx_Arr *self, nx_usize idx);

/// val must not point to an element within the same array.
void nx_arr_set(nx_Arr *self, nx_usize idx, const void *val);

void *nx_arr_data(nx_Arr *self);
const void *nx_arr_data_c(const nx_Arr *self);

/* ========== mods ========== */

void nx_arr_swap(nx_Arr *a, nx_Arr *b);

/* ========== to span ========== */

nx_Span nx_arr_to_span(nx_Arr *self);
nx_CSpan nx_arr_to_cspan(const nx_Arr *self);

/* ========== print ========== */

void nx_arr_fprintln(const nx_Arr *self, FILE *stream, nx_fprint_fn f);
void nx_arr_println(const nx_Arr *self, nx_fprint_fn f);

/* ========== macros ========== */

#define NX_ARR_NEW_LEN(T, len) \
    nx_arr_new_len((len), sizeof(T))

#define NX_ARR_FROM_DATA(T, data, len) \
    nx_arr_from_data((data), (len), sizeof(T))

#define NX_ARR_GET_AS(T, self, idx)              \
    (NX_ASSERT(nx_arr_tsz((self)) == sizeof(T)), \
    (T *) nx_arr_get((self), (idx)))

#define NX_ARR_GET_AS_C(T, self, idx)            \
    (NX_ASSERT(nx_arr_tsz((self)) == sizeof(T)), \
    (const T *) nx_arr_get_c((self), (idx)))

#define NX_ARR_AT_AS(T, self, idx)               \
    (NX_ASSERT(nx_arr_tsz((self)) == sizeof(T)), \
    (T *) nx_arr_at((self), (idx)))

#define NX_ARR_AT_AS_C(T, self, idx)             \
    (NX_ASSERT(nx_arr_tsz((self)) == sizeof(T)), \
    (const T *) nx_arr_at_c((self), (idx)))

#define NX_ARR_SET_EXPR(T, self, idx, expr)         \
    do {                                            \
        NX_ASSERT(nx_arr_tsz((self)) == sizeof(T)); \
        const T nx_tmp_ = (expr);                   \
        nx_arr_set((self), (idx), &nx_tmp_);        \
    } while (0)

#define NX_ARR_DATA_AS(T, self)                  \
    (NX_ASSERT(nx_arr_tsz((self)) == sizeof(T)), \
    (T *) nx_arr_data((self)))

#define NX_ARR_DATA_AS_C(T, self)                \
    (NX_ASSERT(nx_arr_tsz((self)) == sizeof(T)), \
    (const T *) nx_arr_data_c((self)))
