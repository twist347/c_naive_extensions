#pragma once

#include "nx/core/type.h"
#include "nx/core/status.h"
#include "nx/core/result.h"
#include "nx/str/str_view.h"
#include "nx/mem/alloc.h"

/* Contract:
 * - Invalid usage (null, bounds, invariants, type mismatch) is a programmer
 *   error guarded by NX_ASSERT. Release behavior is unspecified.
 *
 * - The only recoverable failure is allocation failure:
 *   - constructors return nx_str_res
 *   - mutating operations return nx_status
 *
 * - Result convention:
 *   - st == NX_STATUS_OK  => val is valid
 *   - st != NX_STATUS_OK  => val must not be used
 *
 * - Null data <--> zero length
 *
 * - Null terminator:
 *   - Always maintained; data[len] == '\0' when data != null
 *   - len and cap exclude the trailing '\0'
 *   - Allocated size is cap + 1
 *
 * - Allocator:
 *   - NOT owned; must outlive the string
 *   - Simple API uses nx_al_libc_default_g()
 *   - For custom allocator, use nx_str_new_p with params
 *
 * - No overflow checking
 */

typedef struct nx_str nx_str;

NX_DEF_RES_TYPE(nx_str_res, nx_str *);

typedef struct {
    nx_usize len; // excludes trailing '\0'
    nx_usize cap; // usable capacity, excludes trailing '\0'
    nx_al *al;    // must not be null
} nx_str_params;

/* ========== lifetime ========== */

nx_str_res nx_str_new_p(nx_str_params p);
nx_str_res nx_str_new(void);
nx_str_res nx_str_new_len(nx_usize len);
nx_str_res nx_str_new_cap(nx_usize cap);
nx_str_res nx_str_from_cstr(const nx_char *cstr);
void nx_str_drop(nx_str *self);

/* ========== copy/move ========== */

nx_str_res nx_str_copy(const nx_str *src);
nx_str_res nx_str_copy_a(const nx_str *src, nx_al *al);
nx_str *nx_str_move(nx_str **src);
nx_status nx_str_copy_assign(nx_str *self, const nx_str *src);
void nx_str_move_assign(nx_str *self, nx_str *src);

/* ========== info ========== */

nx_usize nx_str_len(const nx_str *self);
nx_usize nx_str_cap(const nx_str *self);             /* usable capacity (excludes '\0') */
nx_bool nx_str_empty(const nx_str *self);
nx_al *nx_str_al(const nx_str *self);

/* ========== access ========== */

nx_char nx_str_get(const nx_str *self, nx_usize idx);
nx_char nx_str_at(const nx_str *self, nx_usize idx);
void nx_str_set(nx_str *self, nx_usize idx, nx_char ch);
nx_char *nx_str_data(nx_str *self);                  /* may be NULL if cap==0 */
const nx_char *nx_str_data_c(const nx_str *self);
const nx_char *nx_str_cstr(const nx_str *self);      /* never NULL: returns "" for empty */

/* ========== mods ========== */

void nx_str_clear(nx_str *self);
nx_status nx_str_reserve(nx_str *self, nx_usize new_cap);
nx_status nx_str_resize(nx_str *self, nx_usize new_len);
nx_status nx_str_push(nx_str *self, nx_char ch);
nx_status nx_str_append_str_view(nx_str *self, nx_str_view sv);
nx_status nx_str_append_cstr(nx_str *self, const nx_char *cstr);

/* ========== view ========== */

nx_str_view nx_str_as_view(const nx_str *self);