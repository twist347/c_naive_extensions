#pragma once

#include <stdio.h>

#include "nx/mem/alloc.h"

/**
 * Logging allocator - wraps another allocator and logs all operations
 *
 * Creates heap-allocated allocator with its own heap-allocated context
 * Must be freed with nx_al_free() when done
 *
 * NOTE: The wrapped allocator must outlive this logging allocator
 *
 * @param wrapped - allocator to wrap (must not be NULL)
 * @param stream - where to log (stdout, stderr, or file)
 * @return new logging allocator or NULL on allocation failure
 */
nx_al *nx_al_log_new(nx_al *wrapped, FILE *stream);
void nx_al_log_drop(nx_al *al);
