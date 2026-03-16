#pragma once

#include "nx/core/type.h"
#include "nx/core/status.h"
#include "nx/core/result.h"
#include "nx/core/hash.h"
#include "nx/core/fn.h"
#include "nx/mem/alloc.h"

/* Contract:
 * - Invalid usage (null, invariants) is a programmer error guarded by
 *   NX_ASSERT. Release behavior is unspecified.
 *
 * - The only recoverable failure is allocation failure:
 *   - constructors return nx_MapRes
 *   - put returns nx_Status
 *
 * - Robin Hood open addressing, linear probing, backward-shift delete.
 *   Power-of-2 capacity, max load factor 0.75.
 *
 * - Keys are compared by eq callback. Hashed by hash callback.
 *   Both operate on pointer-to-key (type-erased).
 *
 * - Allocator:
 *   - NOT owned; must outlive the map.
 *   - Simple API uses nx_al_libc_default_g().
 *   - For custom allocator, use nx_map_new_p with params.
 *
 * - Pointers into the map (from get) are invalidated by put and remove.
 */

typedef struct nx_HashMap nx_HashMap;

NX_DEF_RES_TYPE(nx_HashMapRes, nx_HashMap *);

/* ========== params ========== */

typedef struct {
    nx_usize tsz_key;
    nx_usize tsz_val;
    nx_hash_fn hash;
    nx_eq_fn eq;
    nx_Al *al;         // must not be null
} nx_HashMapParams;

/* ========== lifetime ========== */

nx_HashMapRes nx_hash_map_new_p(nx_HashMapParams p);
nx_HashMapRes nx_hash_map_new(nx_usize tsz_key, nx_usize tsz_val, nx_hash_fn hash, nx_eq_fn eq);
void nx_hash_map_drop(nx_HashMap *self);

/* ========== info ========== */

nx_usize nx_hash_map_len(const nx_HashMap *self);
nx_usize nx_hash_map_cap(const nx_HashMap *self);
nx_bool nx_hash_map_empty(const nx_HashMap *self);

/* ========== lookup ========== */

/// returns pointer to value, or null if not found.
void *nx_hash_map_get(nx_HashMap *self, const void *key);
const void *nx_hash_map_get_c(const nx_HashMap *self, const void *key);
nx_bool nx_hash_map_contains(const nx_HashMap *self, const void *key);

/* ========== mutation ========== */

/// insert or overwrite. copies key and value bytes.
nx_Status nx_hash_map_put(nx_HashMap *self, const void *key, const void *val);

/// remove by key. returns true if key existed.
nx_bool nx_hash_map_remove(nx_HashMap *self, const void *key);

/// remove all entries, keep allocated buffer.
void nx_hash_map_clear(nx_HashMap *self);

/// pre-allocate for at least cap entries.
nx_Status nx_hash_map_reserve(nx_HashMap *self, nx_usize cap);

/* ========== iteration ========== */

typedef void (*nx_hash_map_iter_fn)(const void *key, void *val, void *ctx);
typedef void (*nx_hash_map_iter_c_fn)(const void *key, const void *val, void *ctx);

void nx_hash_map_for_each(nx_HashMap *self, nx_hash_map_iter_fn fn, void *ctx);
void nx_hash_map_for_each_c(const nx_HashMap *self, nx_hash_map_iter_c_fn fn, void *ctx);

/* ========== macros ========== */

#define NX_HASH_MAP_NEW(K, V, hash_fn, eq_fn) \
    nx_hash_map_new(sizeof(K), sizeof(V), (hash_fn), (eq_fn))

#define NX_HASH_MAP_PUT(K, V, self, key_expr, val_expr)                   \
    do {                                                                  \
        const K nx_k_ = (key_expr);                                       \
        const V nx_v_ = (val_expr);                                       \
        const nx_Status nx_st_ = nx_hash_map_put((self), &nx_k_, &nx_v_); \
        NX_ASSERT(nx_st_ == NX_STATUS_OK);                                \
    } while (0)

#define NX_HASH_MAP_GET_AS(V, self, key_ptr) \
    ((V *) nx_hash_map_get((self), (key_ptr)))

#define NX_HASH_MAP_GET_AS_C(V, self, key_ptr) \
    ((const V *) nx_hash_map_get_c((self), (key_ptr)))
