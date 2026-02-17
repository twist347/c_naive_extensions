#pragma once

#include <stdio.h>

#include "nx/mem/alloc.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    nx_al wrapped;
    FILE *stream;
} nx_al_log_ctx;

nx_al nx_al_log_new(nx_al_log_ctx *ctx, nx_al wrapped, FILE *stream);

#ifdef __cplusplus
}
#endif
