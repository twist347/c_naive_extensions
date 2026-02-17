#pragma once

#include "nx/core/type.h"
#include "nx/core/status.h"
#include "nx/core/result.h"
#include "nx/str/str_view.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Contract:
 * - Invalid usage is programmer error (NX_ASSERT).
 * - Functions that may fail due to internal reasons report:
 *     - nx_str_res for factories (returning a new nx_str*)
 *     - nx_status for mutating operations
 *   Errors: OUT_OF_MEMORY, MUL_OVERFLOW.
 *
 * Result convention:
 * - st == NX_STATUS_OK  => val is valid
 * - st != NX_STATUS_OK  => val must not be used (NX_RES_GET_VAL asserts)
 */

typedef struct nx_str nx_str;

NX_DEF_RES_TYPE(nx_str_res, nx_str *);

typedef struct nx_str_params {
    nx_usize len;
    nx_usize cap;
} nx_str_params;

/* ========== lifetime ========== */

nx_str_res nx_str_new_p(nx_str_params p);
nx_str_res nx_str_new(void);
nx_str_res nx_str_new_len(nx_usize len);
nx_str_res nx_str_new_cap(nx_usize cap);
nx_str_res nx_str_from_cstr(const nx_char *cstr);
void nx_str_drop(nx_str *self);                                  /* drop(nx_null) = no-op */

/* ========== copy/move ========== */

nx_str_res nx_str_copy(const nx_str *src);
nx_str *nx_str_move(nx_str **src);                               /* take ownership: *src=nx_null */
nx_status nx_str_copy_assign(nx_str *self, const nx_str *src);
void nx_str_move_assign(nx_str *self, nx_str *src);              /* steal buffer, src becomes empty */

/* ========== info ========== */

nx_usize nx_str_len(const nx_str *self);
nx_usize nx_str_cap(const nx_str *self);                         /* usable capacity (excludes '\0') */
nx_bool nx_str_empty(const nx_str *self);

/* ========== access ========== */

nx_char nx_str_get(const nx_str *self, nx_usize idx);
nx_char nx_str_at(const nx_str *self, nx_usize idx);
void nx_str_set(nx_str *self, nx_usize idx, nx_char ch);
nx_char *nx_str_data(nx_str *self); /* may be NULL if cap==0 */
const nx_char *nx_str_data_c(const nx_str *self);
const nx_char *nx_str_cstr(const nx_str *self); /* never NULL: returns "" for empty/unallocated */

/* ========== mods ========== */

void nx_str_clear(nx_str *self);
nx_status nx_str_reserve(nx_str *self, nx_usize new_cap);
nx_status nx_str_resize(nx_str *self, nx_usize new_len);     /* new bytes are '\0' */
nx_status nx_str_push(nx_str *self, nx_char ch);
nx_status nx_str_append_cstr(nx_str *self, const nx_char *cstr);

/* ========== view ========== */

nx_str_view nx_str_as_view(const nx_str *self);

#ifdef __cplusplus
}
#endif
