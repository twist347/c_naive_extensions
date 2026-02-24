cmake_minimum_required(VERSION 3.20)

project(c_naive_extension)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_C_EXTENSIONS OFF) // TODO: wtf? it breaks unity test lib

set(NX_CORE_SOURCES
        src/core/assert.c
        src/core/cmp.c
        src/core/print.c
        src/core/status.c
        src/core/hash.c
)

set(NX_DS_SOURCES
        src/ds/arr.c
        src/ds/vec.c
        src/ds/span.c
)

set(NX_ALGO_SOURCES
        src/algo/bit.c
        src/algo/compare.c
        src/algo/sort.c
        src/algo/search.c
        src/algo/transform.c
        src/algo/minmax.c
)

set(NX_STR_SOURCES
        src/str/str.c
        src/str/str_view.c
)

set(NX_MEM_SOURCES
        src/mem/alloc.c
        src/mem/alloc_arena.c
        src/mem/alloc_libc.c
        src/mem/alloc_log.c
        src/mem/ptr.c
)

set(NX_RAND_SOURCES
        src/rand/rand.c
)

set(NX_NUMERIC_SOURCES
        src/numeric/abs.c
        src/numeric/ckd.c
        src/numeric/sat.c
        src/numeric/sign.c
        src/numeric/bounds.c
)

set(NX_SOURCES
        ${NX_CORE_SOURCES}
        ${NX_DS_SOURCES}
        ${NX_ALGO_SOURCES}
        ${NX_STR_SOURCES}
        ${NX_MEM_SOURCES}
        ${NX_RAND_SOURCES}
        ${NX_NUMERIC_SOURCES}
)

add_library(nx_static STATIC
        ${NX_SOURCES}
)

add_library(nx_shared SHARED
        ${NX_SOURCES}
)

set_target_properties(nx_static PROPERTIES
        OUTPUT_NAME nx
)

set_target_properties(nx_shared PROPERTIES
        OUTPUT_NAME nx
)

#option(NX_SANITIZE "Enable sanitizers" OFF)
#
#if(NX_SANITIZE)
#    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
#    add_link_options(-fsanitize=address,undefined)
#endif()

foreach (target IN ITEMS nx_static nx_shared)
    target_include_directories(${target} PUBLIC ${PROJECT_SOURCE_DIR}/include)
    target_compile_options(${target} PRIVATE -Wall -Wextra -pedantic)
    target_link_libraries(${target} PUBLIC m)
endforeach ()

option(BUILD_TEST "Build unit tests" ON)
option(BUILD_BENCH "Build ubench test" ON)
option(BUILD_DEMO "Build demos" ON)

if (BUILD_TEST)
    set(UNITY_DIR ${PROJECT_SOURCE_DIR}/thirdparty/Unity-2.6.1)
    add_subdirectory(${UNITY_DIR})

    enable_testing()
    add_subdirectory(test)
endif ()

if (BUILD_BENCH)
    set(UBENCH_DIR ${PROJECT_SOURCE_DIR}/thirdparty/ubench)
    add_subdirectory(bench)
endif ()

if (BUILD_DEMO)
    set(DEMOS_DIR demo)
    add_subdirectory(${DEMOS_DIR})
endif ()