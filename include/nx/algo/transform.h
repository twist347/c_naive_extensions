#pragma once

#include "nx/core/type.h"
#include "nx/ds/span.h"

#ifdef __cplusplus
extern "C" {
#endif

void nx_fill(nx_span s, const void *elem);
void nx_reverse(nx_span s);
void nx_swap_elements(nx_span s, nx_usize i, nx_usize j);
void nx_rotate(nx_span s, nx_usize mid);

#ifdef __cplusplus
}
#endif
