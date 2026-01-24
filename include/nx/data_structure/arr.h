#pragma once

#include <stddef.h>
#include <stdbool.h>

#include "nx/core/status.h"
#include "nx/core/span.h"
#include "nx/core/assert.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Contract:
 * - All invalid usage (NULL, bounds, invariants, elem_size mismatch, etc.) is a programmer error:
 *   guarded by NX_ASSERT (debug). Release behavior is unspecified.
 * - Status-returning functions report only internal failures:
 *   OUT_OF_MEMORY and MUL_OVERFLOW.
 */

typedef struct nx_arr nx_arr;

/* ---------- new/destroy ---------- */

nx_status nx_arr_create(nx_arr **out, size_t len, size_t elem_size);
void nx_arr_destroy(nx_arr *self);

/* ---------- copy/move semantic ---------- */

nx_status nx_arr_copy(nx_arr **out, const nx_arr *src);
nx_status nx_arr_move(nx_arr **out, nx_arr *src);
nx_status nx_arr_copy_assign(nx_arr *self, const nx_arr *src);
nx_status nx_arr_move_assign(nx_arr *self, nx_arr *src);

/* ---------- info ---------- */

size_t nx_arr_len(const nx_arr *self);
bool nx_arr_empty(const nx_arr *self);
size_t nx_arr_elem_size(const nx_arr *self);

/* ---------- access ---------- */

void *nx_arr_get(nx_arr *self, size_t idx);
const void *nx_arr_get_const(const nx_arr *self, size_t idx);
void nx_arr_set(nx_arr *self, size_t idx, const void *elem);
void *nx_arr_data(nx_arr *self);
const void *nx_arr_data_const(const nx_arr *self);

/* ---------- mods ---------- */

void nx_arr_swap(nx_arr *a, nx_arr *b);

/* ---------- to span ---------- */

nx_span nx_arr_to_span(nx_arr *self);
nx_cspan nx_arr_to_cspan(const nx_arr *self);

/* ---------- macros ---------- */

#define NX_ARR_CREATE(out, T, len)    \
    nx_arr_create((out), (len), sizeof(T))

#define NX_ARR_GET_PTR(T, self, idx)                      \
    (NX_ASSERT(nx_arr_elem_size((self)) == sizeof(T)),    \
    (T *) nx_arr_get((self), (idx)))

#define NX_ARR_GET_PTR_C(T, self, idx)                    \
    (NX_ASSERT(nx_arr_elem_size((self)) == sizeof(T)),    \
    (const T *) nx_arr_get_const((self), (idx)))

#define NX_ARR_SET(T, self, idx, val)                        \
    do {                                                     \
        NX_ASSERT(nx_arr_elem_size((self)) == sizeof(T));    \
        T nx_tmp_ = (val);                                   \
        nx_arr_set((self), (idx), &nx_tmp_);                 \
    } while (0)

#define NX_ARR_DATA_PTR(T, self)                          \
    (NX_ASSERT(nx_arr_elem_size((self)) == sizeof(T)),    \
    (T *) nx_arr_data((self)))

#define NX_ARR_DATA_PTR_C(T, self)                        \
    (NX_ASSERT(nx_arr_elem_size((self)) == sizeof(T)),    \
    (const T *) nx_arr_data_const((self)))

#ifdef __cplusplus
}
#endif
