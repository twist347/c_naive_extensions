# nx — naive C23 extension library

A small C23 library that extends “plain C” with a few practical building blocks:
basic containers, allocator interfaces, and algorithms built around `span`.

> ⚠️ Unstable API: the project is under active development and may change without preserving compatibility.

---

## Modules

- `nx/core` — base types (`type.h`), `status`, `result`, `assert`, `cmp`, `hash`, misc utilities
- `nx/mem` — allocator interface `nx_Al` + implementations:
    - `alloc_libc` (malloc/calloc/realloc/free)
    - `alloc_arena` (bump arena)
    - `alloc_log` (logging wrapper)
- `nx/ds` — data structures:
    - `span` (non-owning view)
    - `vec`, `arr`
- `nx/string` — `str` (owning string) and `str_view` (non-owning)
- `nx/algo` — algorithms on `span`: sort/search/transform/minmax/compare
- `nx/numeric` — bit/checked/saturating helpers
- `nx/rand` — minimal PRNG
- `nx/io` — minimal input/output facilities

---

## Build

```bash
cmake -S . -B build
cmake --build build