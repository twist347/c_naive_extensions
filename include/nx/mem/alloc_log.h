#pragma once

#include <stdio.h>

#include "nx/mem/alloc.h"

/**
 * Logging allocator - wraps another allocator and logs all operations
 *
 * Creates heap-allocated allocator with its own heap-allocated context
 * Must be freed with nx_al_dealloc() when done
 *
 * NOTE: The wrapped allocator must outlive this logging allocator
 *
 * @param wrapped - allocator to wrap (must not be null)
 * @param stream - where to log (stdout, stderr, or file)
 * @return new logging allocator or null on allocation failure
 */
nx_Al *nx_al_log_new(nx_Al *wrapped, FILE *stream);
void nx_al_log_drop(nx_Al *al);
