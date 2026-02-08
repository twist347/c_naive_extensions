#include <stdlib.h>   // atexit
#include <stdint.h>

#include "ubench.h"

#include "nx/data_structure/vec.h"

#ifndef NX_BENCH_N
#define NX_BENCH_N 1000000u
#endif

static nx_vec *g_vec = nx_null;

static void bench_cleanup(void) {
    if (g_vec) {
        nx_vec_drop(g_vec);
        g_vec = nx_null;
    }
}

UBENCH(vec, push_int_reserved) {
    nx_vec_clear(g_vec);

    for (nx_usize i = 0; i < (nx_usize) NX_BENCH_N; ++i) {
        int x = (int) i;
        const nx_status st = nx_vec_push(g_vec, &x);
        if (st != NX_STATUS_OK) {
            return;
        }
    }

    nx_usize len = nx_vec_len(g_vec);
    UBENCH_DO_NOTHING(&len);
}

UBENCH(vec, push_int_no_reserve) {
    const nx_vec_res r = NX_VEC_NEW(int);
    if (r.st != NX_STATUS_OK) {
        return;
    }

    nx_vec *v = r.val;

    for (nx_usize i = 0; i < (nx_usize)NX_BENCH_N; ++i) {
        int x = (int)i;
        const nx_status st = nx_vec_push(v, &x);
        if (st != NX_STATUS_OK) break;
    }

    nx_usize len = nx_vec_len(v);
    UBENCH_DO_NOTHING(&len);

    nx_vec_drop(v);
}

UBENCH_STATE();

int main(int argc, const char *const argv[]) {
    const nx_vec_res r = NX_VEC_NEW_CAP(int, NX_BENCH_N);
    if (r.st != NX_STATUS_OK) {
        return 1;
    }

    g_vec = r.val;
    atexit(bench_cleanup);

    return ubench_main(argc, argv);
}
