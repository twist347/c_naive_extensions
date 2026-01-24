#pragma once

#include <stddef.h>
#include <stdbool.h>

#include "nx/core/span.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nx_vec nx_vec;

/* ---------- new/destroy ---------- */

nx_vec *nx_vec_create(size_t len, size_t elem_size);
nx_vec *nx_vec_create_cap(size_t cap, size_t elem_size);
void nx_vec_destroy(nx_vec *self);

/* ---------- copy/move semantic ---------- */

nx_vec *nx_vec_copy(const nx_vec *src);
nx_vec *nx_vec_move(nx_vec *src);
bool nx_vec_copy_assign(nx_vec *self, const nx_vec *src);
void nx_vec_move_assign(nx_vec *self, nx_vec *src);

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

bool nx_vec_push(nx_vec *self, const void *elem);
void *nx_vec_pop(nx_vec *self);
void nx_vec_clear(nx_vec *self);
bool nx_vec_reserve(nx_vec *self, size_t new_cap);
bool nx_vec_resize(nx_vec *self, size_t new_len);
bool nx_vec_shrink_to_fit(nx_vec *self);
void nx_vec_swap(nx_vec *a, nx_vec *b);
bool nx_vec_insert(nx_vec *self, size_t idx, const void *elem);
void nx_vec_erase(nx_vec *self, size_t idx);

/* ---------- to span ---------- */

nx_span nx_vec_to_span(nx_vec *self);
nx_cspan nx_vec_to_cspan(const nx_vec *self);

#ifdef __cplusplus
}
#endif

