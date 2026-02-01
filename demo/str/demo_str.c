#include "nx/str/str.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    const nx_str_res res = nx_str_new();
    if (!NX_RES_IS_OK(res)) {
        nx_status_fprint(stderr, NX_RES_GET_ERR(res));
        return EXIT_FAILURE;
    }

    nx_str *str = NX_RES_GET_VAL(res);

    for (nx_char c = 'a'; c <= 'z'; ++c) {
        nx_str_push(str, c);
    }

    for (nx_usize i = 0; i < nx_str_len(str); ++i) {
        printf("%c ", nx_str_get(str, i));
    }

    printf("\n");

    nx_str_drop(str);
}
