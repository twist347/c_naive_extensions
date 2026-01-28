#pragma once

#include <stdio.h>
#include <stdlib.h>

#define NX_VERIFY_MSG(cond, msg)                                        \
    do {                                                                \
        if (!(cond)) {                                                  \
            fprintf(                                                    \
                stderr,                                                 \
                "[NX]: verify failed: %s. file: %s, line: %d. %s\n",    \
                #cond, __FILE__, __LINE__, (msg) ? (msg) : ""           \
            );                                                          \
            fflush(stderr);                                             \
            abort();                                                    \
        }                                                               \
    } while (0)

#define NX_VERIFY(cond) NX_VERIFY_MSG((cond), "")

#define NX_UNIMPLEMENTED()                          \
    do {                                            \
        fprintf(                                    \
            stderr,                                 \
            "[NX]: unimplemented: %s (%s:%d)\n",    \
            __func__, __FILE__, __LINE__            \
        );                                          \
        fflush(stderr);                             \
        abort();                                    \
    } while (0)

#define NX_UNREACHABLE()                          \
    do {                                          \
        fprintf(                                  \
            stderr,                               \
            "[NX]: unreachable: %s (%s:%d)\n",    \
            __func__, __FILE__, __LINE__          \
        );                                        \
        fflush(stderr);                           \
        abort();                                  \
    } while (0)
