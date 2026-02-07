#include "criterion/criterion.h"

#include "nx/core/hash.h"
#include "nx/core/print.h"

Test(A, B) {
    for (nx_i32 i = -10; i < 10; ++i) {
        const nx_hash h = nx_hash_i32(i);
        nx_fprintf(stdout, NX_FMT_HASH "\n", h);
    }

    nx_fprintf(stdout, NX_FMT_HASH "\n", nx_hash_cstr("hello"));
}
