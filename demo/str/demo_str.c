#include "nx/string/str.h"

#include <stdlib.h>

#include "nx/io/print.h"

int main(void) {
    const nx_StrRes res = nx_str_new();
    if (!NX_RES_IS_OK(res)) {
        nx_fprintf(stderr, nx_status_to_str(NX_RES_ERR(res)));
        return EXIT_FAILURE;
    }

    nx_Str *str = NX_RES_UNWRAP(res);

    for (nx_char c = 'a'; c <= 'z'; ++c) {
        nx_str_push(str, c);
    }

    for (nx_usize i = 0; i < nx_str_len(str); ++i) {
        printf("%c ", nx_str_get(str, i));
    }

    printf("\n");

    nx_str_drop(str);
}
