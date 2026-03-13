#pragma once

#include "nx/core/type.h"

/// Comparator: <0 if a<b, 0 if a==b, >0 if a>b. qsort-compatible.
typedef int (*nx_cmp_fn)(const void *, const void *);

/// Equality: true if a == b.
typedef nx_bool (*nx_eq_fn)(const void *, const void *);

/// Unary predicate: true if elem satisfies condition.
typedef nx_bool (*nx_predicate_fn)(const void *elem);

/// Unary transform: write result into dst from src. dst and src have equal tsz.
typedef void (*nx_transform_fn)(void *dst, const void *src);

/// Generator: write a value into out.
typedef void (*nx_generate_fn)(void *out);

/// Mutable visitor.
typedef void (*nx_for_each_fn)(void *elem);

/// Const visitor.
typedef void (*nx_for_each_c_fn)(const void *elem);
