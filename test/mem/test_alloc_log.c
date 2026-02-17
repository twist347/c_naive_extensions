#include <stdio.h>

#include "nx/core/print.h"
#include "nx/mem/alloc_libc.h"
#include "nx/mem/alloc_log.h"

int main(void) {
    const nx_al libc_alloc = nx_al_libc_new();

    nx_al_log_ctx log_ctx = {0};
    const nx_al log_alloc = nx_al_log_new(&log_ctx, libc_alloc, stdout);

    nx_i32 *arr = nx_al_alloc(log_alloc, 5 * sizeof(nx_i32));

    for (nx_i32 i = 0; i < 5; ++i) {
        arr[i] = i;
    }

    for (nx_i32 i = 0; i < 5; ++i) {
        printf("arr[%d] = %d ", i, arr[i]);
    }
    printf("\n");

    nx_i32 *p = nx_al_realloc(log_alloc, arr, 5 * sizeof(nx_i32), 10 * sizeof(nx_i32));
    if (p) {
        arr = p;
    }

    for (nx_i32 i = 0; i < 10; ++i) {
        arr[i] = i * i;
    }

    for (nx_i32 i = 0; i < 10; ++i) {
        printf("arr[%d] = %d ", i, arr[i]);
    }
    printf("\n");

    nx_al_dealloc(log_alloc, arr, 5 * sizeof(nx_i32));
}
