# nx — naive C23 extension library

A small C23 library that extends “plain C” with a few practical building blocks:
basic containers, allocator interfaces, and algorithms built around `span`.

> ⚠️ Unstable API: the project is under active development and may change without preserving compatibility.

---

## Modules

- `nx/core` — base types (`type.h`), `status`, `result`, `optional`, `assert`, `cmp`, `hash`, function pointer types (`fn.h`), misc utilities
- `nx/mem` — allocator interface `nx_Al` + implementations:
    - `alloc_libc` (malloc/calloc/realloc/free)
    - `alloc_arena` (bump arena)
    - `alloc_pool` (fixed-size block pool)
    - `alloc_log` (logging wrapper)
    - `ptr` (pointer arithmetic, alignment, memory operations)
- `nx/ds` — data structures:
    - `span` / `cspan` (non-owning mutable/const views)
    - `vec` (dynamic array)
    - `arr` (fixed-length array)
    - `hash_map` (robin hood open addressing)
- `nx/string` — `str` (owning, null-terminated) and `str_view` (non-owning)
- `nx/algo` — algorithms on `span`:
    - sort (quicksort, stable merge sort, partial sort, nth element)
    - search (linear, binary, lower/upper bound, predicates)
    - transform (fill, reverse, rotate, copy, replace, remove, unique)
    - merge (sorted merge, in-place merge)
    - compare, minmax
- `nx/numeric` — numeric utilities:
    - checked arithmetic (`ckd`) — overflow detection
    - saturating arithmetic (`sat`)
    - bit operations (count, rotate, swap, extract, deposit)
    - abs, sign, bounds (min/max/clamp)
    - floating point (lerp, inverse lerp, approximate equality)
    - mathematical constants
- `nx/fs` — file system utilities (read file to string/bytes, write, append, exists, file size)
- `nx/rand` — PRNG (integers, floats, ranges, shuffle)
- `nx/io` — input/output:
    - `print` — formatted and typed printers
    - `dbg` — debug expression printing (file, line, value)
    - `log` — leveled logging (DEBUG/INFO/WARN/ERROR) with compile-time and runtime filtering

---
## Build

```bash
nix develop          # GCC (default)
nix develop .#clang  # Clang
```

```bash
cmake -S . -B build
cmake --build build
```