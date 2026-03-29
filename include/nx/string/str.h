#pragma once

#include <stdio.h>

#include "nx/core/type.h"
#include "nx/core/status.h"
#include "nx/core/result.h"
#include "nx/string/str_view.h"
#include "nx/mem/alloc.h"

// TODO: add print funcs

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

typedef struct nx_Str nx_Str;

NX_DEF_RES_TYPE(nx_StrRes, nx_Str *);

typedef struct {
    nx_usize len; // excludes trailing '\0'
    nx_usize cap; // usable capacity, excludes trailing '\0'
    nx_Al *al;    // must not be null
} nx_StrParams;

/* ========== lifetime ========== */

nx_StrRes nx_str_new_p(nx_StrParams p);

/// empty string, no allocation until first push/append.
nx_StrRes nx_str_new(void);

/// zero-initialized string of length len.
nx_StrRes nx_str_new_len(nx_usize len);

/// empty string with pre-allocated capacity.
nx_StrRes nx_str_new_cap(nx_usize cap);

/// copies from null-terminated C string.
nx_StrRes nx_str_from_cstr(const nx_char *cstr);

/// frees memory. safe to call with null.
void nx_str_drop(nx_Str *self);

/* ========== copy/move ========== */

/// deep copy with same allocator.
nx_StrRes nx_str_copy(const nx_Str *src);

/// deep copy using a different allocator.
nx_StrRes nx_str_copy_a(const nx_Str *src, nx_Al *al);

/// transfers ownership: returns *src, sets *src to null.
nx_Str *nx_str_move(nx_Str **src);

/// deep copy into existing str. reuses buffer if it fits.
nx_Status nx_str_copy_assign(nx_Str *self, const nx_Str *src);

/// transfers src's data into self. src becomes empty. same allocator required.
void nx_str_move_assign(nx_Str *self, nx_Str *src);

/* ========== info ========== */

nx_usize nx_str_len(const nx_Str *self);
nx_usize nx_str_cap(const nx_Str *self);             /* usable capacity (excludes '\0') */
nx_bool nx_str_empty(const nx_Str *self);
nx_Al *nx_str_al(const nx_Str *self);

/* ========== access ========== */

/// get: unchecked in release (asserts in debug).
nx_char nx_str_get(const nx_Str *self, nx_usize idx);

/// at: returns '\0' if idx is out of bounds.
nx_char nx_str_at(const nx_Str *self, nx_usize idx);
void nx_str_set(nx_Str *self, nx_usize idx, nx_char ch);
nx_char *nx_str_data(nx_Str *self);                  /* may be NULL if cap==0 */
const nx_char *nx_str_data_c(const nx_Str *self);
const nx_char *nx_str_cstr(const nx_Str *self);      /* never NULL: returns "" for empty */

/* ========== mods ========== */

/// sets len to 0 without freeing memory. data[0] = '\0'.
void nx_str_clear(nx_Str *self);

/// ensures capacity >= new_cap. no-op if already sufficient.
nx_Status nx_str_reserve(nx_Str *self, nx_usize new_cap);

/// sets len to new_len. new chars are zero-filled. may reallocate.
nx_Status nx_str_resize(nx_Str *self, nx_usize new_len);

/// appends single char.
nx_Status nx_str_push(nx_Str *self, nx_char ch);

/// appends contents of str_view.
nx_Status nx_str_append_str_view(nx_Str *self, nx_StrView sv);

/// appends null-terminated C string.
nx_Status nx_str_append_cstr(nx_Str *self, const nx_char *cstr);

/* ========== print ========== */

void nx_str_fprint(FILE *stream, const nx_Str *self);
void nx_str_fprintln(FILE *stream, const nx_Str *self);

void nx_str_print(const nx_Str *self);
void nx_str_println(const nx_Str *self);

/* ========== view ========== */

nx_StrView nx_str_as_view(const nx_Str *self);
