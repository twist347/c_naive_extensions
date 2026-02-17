#include <stdio.h>

#include "nx/mem/alloc_libc.h"

int main(void) {
    const nx_al alloc = nx_al_libc_new();

    nx_i32 *x = nx_al_alloc(alloc, sizeof(nx_i32));

    *x = 123;

    printf("%d\n", *x);

    nx_al_dealloc(alloc, x, sizeof(nx_i32));
}
