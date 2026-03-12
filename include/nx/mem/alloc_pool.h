#pragma once

#include "nx/mem/alloc.h"

/**
 * Pool allocator — fixed-size block allocator with O(1) alloc/dealloc.
 *
 * All blocks have the same size (aligned to max_align_t).
 * Free blocks form an intrusive singly-linked list, so alloc and dealloc
 * are both O(1) with zero fragmentation.
 *
 * Constraints:
 * - alloc succeeds only when size <= block_size
 * - calloc succeeds only when num * size <= block_size (single block)
 * - realloc is not provided (falls back to alloc + memcpy + dealloc in nx_al)
 * - dealloc returns the block to the free list; ptr must have been
 *   allocated from this pool
 *
 * Use cases:
 * - Many allocations of the same type (nodes, entries, handles)
 * - Object pools for frequently created/destroyed objects
 * - Allocation-heavy containers with uniform element size
 *
 * @param block_size - usable size per block (will be aligned up, minimum sizeof(void *))
 * @param block_count - total number of blocks to pre-allocate
 * @return new pool allocator or NULL on allocation failure
 */

nx_Al *nx_al_pool_new(nx_usize block_size, nx_usize block_count);
void nx_al_pool_drop(nx_Al *al);

/**
 * Reset pool to fully-free state.
 * All previous allocations become invalid.
 * The underlying buffer is reused without reallocation.
 */
void nx_al_pool_reset(nx_Al *al);

typedef struct {
    nx_usize block_size;  // aligned block size in bytes
    nx_usize block_count; // total number of blocks
    nx_usize used;        // currently allocated blocks
    nx_usize free;        // currently free blocks
} nx_AlPoolStats;

nx_AlPoolStats nx_al_pool_get_stats(const nx_Al *al);
