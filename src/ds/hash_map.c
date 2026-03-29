#include "nx/ds/hash_map.h"

#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "nx/core/assert.h"
#include "nx/core/limits.h"
#include "nx/numeric/ckd.h"
#include "nx/mem/alloc_libc.h"
#include "nx/mem/ptr.h"

/* ========== constants ========== */

constexpr nx_Hash EMPTY_HASH = 0;
constexpr nx_usize SLOT_ALIGN = alignof(max_align_t);
constexpr nx_usize DEFAULT_CAP = 8;
// rehash when len * 4 >= cap * 3  (75%)
constexpr nx_usize LOAD_NUM = 3;
constexpr nx_usize LOAD_DEN = 4;

/* ========== layout ========== */

typedef struct {
    nx_usize key_off;
    nx_usize val_off;
    nx_usize slot_size;
} nx_SlotLayout;

static nx_SlotLayout make_layout(nx_usize tsz_key, nx_usize tsz_val);

/* ========== struct ========== */

struct nx_HashMap {
    nx_byte *slots;
    nx_usize cap; // power of 2
    nx_usize len;
    nx_usize tsz_key;
    nx_usize tsz_val;
    nx_SlotLayout layout;
    nx_hash_fn hash;
    nx_eq_fn eq;
    nx_Al *al;
};

static nx_byte *slot_ptr(const nx_HashMap *m, nx_usize idx);
static nx_Hash *slot_hash(const nx_HashMap *m, nx_usize idx);
static void *slot_key(const nx_HashMap *m, nx_usize idx);
static void *slot_val(const nx_HashMap *m, nx_usize idx);
static nx_Hash fix_hash(nx_Hash h);
static nx_usize probe_dist(nx_usize cap, nx_Hash h, nx_usize idx);

static nx_Status alloc_slots(nx_HashMap *m, nx_usize cap);
static nx_Status grow_and_rehash(nx_HashMap *m);
static void insert_no_grow(nx_HashMap *m, nx_Hash h, const void *key, const void *val);
static nx_isize find_slot(const nx_HashMap *m, nx_Hash h, const void *key);
static nx_bool needs_grow(const nx_HashMap *m);

/* ========== debug assert ========== */

#if NX_DEBUG
    static void map_assert(const nx_HashMap *self) {
        NX_ASSERT(self != nx_null);
        NX_ASSERT(self->tsz_key > 0);
        NX_ASSERT(self->tsz_val > 0);
        NX_ASSERT(self->hash != nx_null);
        NX_ASSERT(self->eq != nx_null);
        NX_ASSERT(self->al != nx_null);
        NX_ASSERT(self->len <= self->cap);
        NX_ASSERT((self->cap == 0) == (self->slots == nx_null));
        // cap is 0 or power of 2
        NX_ASSERT(self->cap == 0 || (self->cap & (self->cap - 1)) == 0);
    }

    #define MAP_ASSERT(self) do { map_assert(self); } while (0)
#else
    #define MAP_ASSERT(self) ((void) 0)
#endif

nx_HashMapRes nx_hash_map_new_p(nx_HashMapParams p) {
    NX_ASSERT(p.tsz_key > 0);
    NX_ASSERT(p.tsz_val > 0);
    NX_ASSERT(p.hash);
    NX_ASSERT(p.eq);
    NX_ASSERT(p.al);

    nx_HashMap *m = malloc(sizeof(nx_HashMap));
    if (!m) {
        return NX_RES_NEW_ERR(nx_HashMapRes, NX_STATUS_OUT_OF_MEMORY);
    }

    m->slots = nx_null;
    m->cap = 0;
    m->len = 0;
    m->tsz_key = p.tsz_key;
    m->tsz_val = p.tsz_val;
    m->layout = make_layout(p.tsz_key, p.tsz_val);
    m->hash = p.hash;
    m->eq = p.eq;
    m->al = p.al;

    return NX_RES_NEW_OK(nx_HashMapRes, m);
}

nx_HashMapRes nx_hash_map_new(nx_usize tsz_key, nx_usize tsz_val, nx_hash_fn hash, nx_eq_fn eq) {
    return nx_hash_map_new_p((nx_HashMapParams){
        .tsz_key = tsz_key,
        .tsz_val = tsz_val,
        .hash = hash,
        .eq = eq,
        .al = nx_al_libc_default_g(),
    });
}

void nx_hash_map_drop(nx_HashMap *self) {
    if (!self) {
        return;
    }
    if (self->slots) {
        nx_al_dealloc(self->al, self->slots, self->cap * self->layout.slot_size);
    }
    free(self);
}

/* ========== info ========== */

nx_usize nx_hash_map_len(const nx_HashMap *self) {
    MAP_ASSERT(self);
    return self->len;
}

nx_usize nx_hash_map_cap(const nx_HashMap *self) {
    MAP_ASSERT(self);
    return self->cap;
}

nx_bool nx_hash_map_empty(const nx_HashMap *self) {
    MAP_ASSERT(self);
    return self->len == 0;
}

/* ========== lookup ========== */

void *nx_hash_map_get(nx_HashMap *self, const void *key) {
    MAP_ASSERT(self);
    NX_ASSERT(key);

    if (self->len == 0) {
        return nx_null;
    }

    const nx_Hash h = fix_hash(self->hash(key));
    const nx_isize idx = find_slot(self, h, key);

    return idx >= 0 ? slot_val(self, (nx_usize) idx) : nx_null;
}

const void *nx_hash_map_get_c(const nx_HashMap *self, const void *key) {
    // const_cast is safe — we return const pointer
    return nx_hash_map_get((nx_HashMap *) self, key);
}

nx_bool nx_hash_map_contains(const nx_HashMap *self, const void *key) {
    return nx_hash_map_get_c(self, key) != nx_null;
}

/* ========== mutation ========== */

nx_Status nx_hash_map_put(nx_HashMap *self, const void *key, const void *val) {
    MAP_ASSERT(self);
    NX_ASSERT(key);
    NX_ASSERT(val);

    const nx_Hash h = fix_hash(self->hash(key));

    // check for existing key (overwrite)
    if (self->len > 0) {
        const nx_isize idx = find_slot(self, h, key);
        if (idx >= 0) {
            memcpy(slot_val(self, (nx_usize) idx), val, self->tsz_val);
            return NX_STATUS_OK;
        }
    }

    // new entry — may need to grow
    if (needs_grow(self)) {
        const nx_Status st = grow_and_rehash(self);
        if (st != NX_STATUS_OK) {
            return st;
        }
    }

    insert_no_grow(self, h, key, val);
    ++self->len;
    return NX_STATUS_OK;
}

nx_bool nx_hash_map_remove(nx_HashMap *self, const void *key) {
    MAP_ASSERT(self);
    NX_ASSERT(key);

    if (self->len == 0) {
        return false;
    }

    const nx_Hash h = fix_hash(self->hash(key));
    const nx_isize idx = find_slot(self, h, key);

    if (idx < 0) {
        return false;
    }

    // backward-shift delete
    nx_usize cur = (nx_usize) idx;
    const nx_usize mask = self->cap - 1;

    while (true) {
        const nx_usize next = (cur + 1) & mask;
        const nx_Hash nh = *slot_hash(self, next);

        // stop if next slot is empty or at its ideal position
        if (nh == EMPTY_HASH || probe_dist(self->cap, nh, next) == 0) {
            *slot_hash(self, cur) = EMPTY_HASH;
            break;
        }

        // shift next back to cur
        memcpy(slot_ptr(self, cur), slot_ptr(self, next), self->layout.slot_size);
        cur = next;
    }

    --self->len;
    return true;
}

void nx_hash_map_clear(nx_HashMap *self) {
    MAP_ASSERT(self);

    if (self->slots && self->cap > 0) {
        memset(self->slots, 0, self->cap * self->layout.slot_size);
    }
    self->len = 0;
}

nx_Status nx_hash_map_reserve(nx_HashMap *self, nx_usize cap) {
    MAP_ASSERT(self);

    // compute needed table capacity for `cap` entries at max load
    nx_usize table_cap = self->cap == 0 ? DEFAULT_CAP : self->cap;
    while (cap * LOAD_DEN >= table_cap * LOAD_NUM) {
        nx_usize new_cap;
        if (nx_ckd_mul_usize(&new_cap, table_cap, 2)) {
            return NX_STATUS_OUT_OF_MEMORY;
        }
        table_cap = new_cap;
    }

    if (table_cap <= self->cap) {
        return NX_STATUS_OK;
    }

    // allocate new table and rehash
    nx_byte *old_slots = self->slots;
    const nx_usize old_cap = self->cap;

    const nx_Status st = alloc_slots(self, table_cap);
    if (st != NX_STATUS_OK) {
        return st;
    }

    // re-insert old entries
    if (old_slots) {
        for (nx_usize i = 0; i < old_cap; ++i) {
            const nx_Hash *ph = (nx_Hash *) (old_slots + i * self->layout.slot_size);
            if (*ph != EMPTY_HASH) {
                const void *k = old_slots + i * self->layout.slot_size + self->layout.key_off;
                const void *v = old_slots + i * self->layout.slot_size + self->layout.val_off;
                insert_no_grow(self, *ph, k, v);
            }
        }
        nx_al_dealloc(self->al, old_slots, old_cap * self->layout.slot_size);
    }

    return NX_STATUS_OK;
}

/* ========== iteration ========== */

void nx_hash_map_for_each(nx_HashMap *self, nx_hash_map_iter_fn fn, void *ctx) {
    MAP_ASSERT(self);
    NX_ASSERT(fn);

    for (nx_usize i = 0; i < self->cap; ++i) {
        if (*slot_hash(self, i) != EMPTY_HASH) {
            fn(slot_key(self, i), slot_val(self, i), ctx);
        }
    }
}

void nx_hash_map_for_each_c(const nx_HashMap *self, nx_hash_map_iter_c_fn fn, void *ctx) {
    MAP_ASSERT(self);
    NX_ASSERT(fn);

    for (nx_usize i = 0; i < self->cap; ++i) {
        if (*slot_hash(self, i) != EMPTY_HASH) {
            fn(slot_key(self, i), slot_val(self, i), ctx);
        }
    }
}


// inner defs

static nx_SlotLayout make_layout(nx_usize tsz_key, nx_usize tsz_val) {
    constexpr nx_usize key_off = sizeof(nx_Hash); // 8, max-aligned
    const nx_usize val_off = nx_align_up(key_off + tsz_key, SLOT_ALIGN);
    const nx_usize slot_size = nx_align_up(val_off + tsz_val, SLOT_ALIGN);
    return (nx_SlotLayout){
        .key_off = key_off,
        .val_off = val_off,
        .slot_size = slot_size
    };
}

static nx_byte *slot_ptr(const nx_HashMap *m, nx_usize idx) {
    return m->slots + idx * m->layout.slot_size;
}

static nx_Hash *slot_hash(const nx_HashMap *m, nx_usize idx) {
    return (nx_Hash *) slot_ptr(m, idx);
}

static void *slot_key(const nx_HashMap *m, nx_usize idx) {
    return slot_ptr(m, idx) + m->layout.key_off;
}

static void *slot_val(const nx_HashMap *m, nx_usize idx) {
    return slot_ptr(m, idx) + m->layout.val_off;
}

static nx_Hash fix_hash(nx_Hash h) {
    // 0 is reserved for empty
    return h == EMPTY_HASH ? 1 : h;
}

static nx_usize probe_dist(nx_usize cap, nx_Hash h, nx_usize idx) {
    const nx_usize ideal = h & (cap - 1);
    return (idx - ideal) & (cap - 1);
}

static nx_Status alloc_slots(nx_HashMap *m, nx_usize cap) {
    NX_ASSERT(cap > 0);
    NX_ASSERT((cap & (cap - 1)) == 0);

    const nx_usize bytes = cap * m->layout.slot_size;
    nx_byte *slots = nx_al_alloc(m->al, bytes);
    if (!slots) {
        return NX_STATUS_OUT_OF_MEMORY;
    }

    memset(slots, 0, bytes); // all hashes = 0 => empty
    m->slots = slots;
    m->cap = cap;
    return NX_STATUS_OK;
}

static nx_bool needs_grow(const nx_HashMap *m) {
    if (m->cap == 0) {
        return true;
    }
    // len * 4 >= cap * 3
    return m->len * LOAD_DEN >= m->cap * LOAD_NUM;
}

static nx_Status grow_and_rehash(nx_HashMap *m) {
    const nx_usize new_cap = m->cap == 0 ? DEFAULT_CAP : m->cap * 2;

    // TODO: check overrflow

    nx_byte *old_slots = m->slots;
    const nx_usize old_cap = m->cap;

    const nx_Status st = alloc_slots(m, new_cap);
    if (st != NX_STATUS_OK) {
        // restore
        m->slots = old_slots;
        m->cap = old_cap;
        return st;
    }

    // re-insert old entries
    if (old_slots) {
        for (nx_usize i = 0; i < old_cap; ++i) {
            const nx_Hash *ph = (nx_Hash *) (old_slots + i * m->layout.slot_size);
            if (*ph != EMPTY_HASH) {
                const void *k = old_slots + i * m->layout.slot_size + m->layout.key_off;
                const void *v = old_slots + i * m->layout.slot_size + m->layout.val_off;
                insert_no_grow(m, *ph, k, v);
            }
        }
        nx_al_dealloc(m->al, old_slots, old_cap * m->layout.slot_size);
    }

    return NX_STATUS_OK;
}

static void insert_no_grow(nx_HashMap *m, nx_Hash h, const void *key, const void *val) {
    NX_ASSERT(m->cap > 0);
    NX_ASSERT(h != EMPTY_HASH);

    const nx_usize mask = m->cap - 1;
    nx_usize idx = h & mask;

    // temp storage for displaced entry
    const nx_Hash cur_h = h;
    // we'll use small stack buffers to hold displaced key/val
    // since tsz_key/tsz_val are runtime values, use alloca-like approach
    // but safer: just memcpy through the slot directly

    // We need to "carry" a displaced entry. Use a temp slot buffer.
    nx_byte tmp[m->layout.slot_size]; // VLA
    memcpy(tmp + m->layout.key_off, key, m->tsz_key);
    memcpy(tmp + m->layout.val_off, val, m->tsz_val);
    *(nx_Hash *) tmp = cur_h;

    while (true) {
        const nx_Hash existing_h = *slot_hash(m, idx);

        if (existing_h == EMPTY_HASH) {
            // place here
            memcpy(slot_ptr(m, idx), tmp, m->layout.slot_size);
            return;
        }

        // Robin Hood: steal from the rich (shorter probe distance)
        const nx_usize existing_dist = probe_dist(m->cap, existing_h, idx);
        const nx_usize our_dist = probe_dist(m->cap, *(nx_Hash *) tmp, idx);

        if (our_dist > existing_dist) {
            // swap our entry with existing
            nx_byte swap[m->layout.slot_size]; // VLA
            memcpy(swap, slot_ptr(m, idx), m->layout.slot_size);
            memcpy(slot_ptr(m, idx), tmp, m->layout.slot_size);
            memcpy(tmp, swap, m->layout.slot_size);
        }

        idx = (idx + 1) & mask;
    }
}

static nx_isize find_slot(const nx_HashMap *m, nx_Hash h, const void *key) {
    NX_ASSERT(m->cap > 0);
    NX_ASSERT(h != EMPTY_HASH);

    const nx_usize mask = m->cap - 1;
    nx_usize idx = h & mask;
    nx_usize dist = 0;

    while (true) {
        const nx_Hash sh = *slot_hash(m, idx);

        if (sh == EMPTY_HASH) {
            return -1;
        }

        // if our probe distance exceeds this slot's, key doesn't exist
        // (Robin Hood invariant)
        if (dist > probe_dist(m->cap, sh, idx)) {
            return -1;
        }

        if (sh == h && m->eq(slot_key(m, idx), key)) {
            return (nx_isize) idx;
        }

        idx = (idx + 1) & mask;
        ++dist;
    }
}
