#pragma once

#include "nx/core/type.h"
#include "nx/ds/span.h"

void nx_fill(nx_span s, const void *elem);
void nx_reverse(nx_span s);
void nx_swap_elements(nx_span s, nx_usize i, nx_usize j);
void nx_rotate(nx_span s, nx_usize mid);

typedef void (*nx_transform_fn)(void *dst, const void *src);
void nx_transform(nx_span dst, nx_cspan src, nx_transform_fn fn);

typedef void (nx_apply_fn)(void *val);
void nx_apply(nx_span s, nx_apply_fn fn);
