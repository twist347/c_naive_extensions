#pragma once

#include "nx/mem/alloc.h"

/**
 * Create libc allocator (malloc/calloc/realloc/free wrapper)
 *
 * Returns heap-allocated allocator that must be freed with nx_al_free()
 * This allocator has no state (ctx = NULL)
 */

nx_al *nx_al_libc_new(void);
void nx_al_libc_drop(nx_al *al);

/**
 * Get global static libc allocator
 *
 * Returns pointer to static allocator - DO NOT free it!
 * Use this when you need a default allocator without allocation overhead
 *
 * This is the recommended default allocator for most use cases
 *
 * DO NOT modify fields.
 */
nx_al *nx_al_libc_default_g(void);
