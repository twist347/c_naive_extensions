#include "nx/data_structure/vec.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    nx_vec *vec = nx_null;

    const nx_status st = NX_VEC_NEW(&vec, int, 5);
    if (st != NX_STATUS_OK) {
        nx_status_fprint(stderr, st);
        return EXIT_FAILURE;
    }

    nx_vec_drop(vec);
}
