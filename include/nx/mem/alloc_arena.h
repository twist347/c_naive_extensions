#pragma once

#include "nx/mem/alloc.h"

/**
 * Arena allocator - bump allocator with fixed-size buffer
 *
 * Features:
 * - Very fast allocation (just bump offset)
 * - No individual deallocations (dealloc is no-op)
 * - All memory freed when arena is destroyed or reset
 * - realloc copies to new location (old memory is lost until reset)
 *
 * Use cases:
 * - Per-frame allocations in game loop
 * - Request/response processing
 * - Temporary data structures with known lifetime
 *
 * @param cap - size of arena buffer in bytes
 * @return new arena allocator or NULL on allocation failure
 */
nx_al *nx_al_arena_new(nx_usize cap);

/**
 * Reset arena to empty state (offset = 0)
 * All previous allocations become invalid
 * Buffer is reused without reallocation
 */
void nx_al_arena_reset(nx_al *al);

typedef struct {
    nx_usize cap;        // total arena capacity
    nx_usize used;       // currently used bytes
    nx_usize available;  // available bytes
} nx_al_arena_stats;

nx_al_arena_stats nx_al_arena_get_stats(const nx_al *al);
