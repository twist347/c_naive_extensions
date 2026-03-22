cmake_minimum_required(VERSION 3.21)

project(c_naive_extension LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(NX_CORE_SOURCES
        src/core/assert.c
        src/core/cmp.c
        src/core/status.c
        src/core/hash.c
)

set(NX_DS_SOURCES
        src/ds/arr.c
        src/ds/hash_map.c
        src/ds/vec.c
        src/ds/span.c
)

set(NX_ALGO_SOURCES
        src/algo/compare.c
        src/algo/sort.c
        src/algo/search.c
        src/algo/transform.c
        src/algo/minmax.c
)

set(NX_STRING_SOURCES
        src/string/str.c
        src/string/str_view.c
)

set(NX_MEM_SOURCES
        src/mem/alloc.c
        src/mem/alloc_arena.c
        src/mem/alloc_libc.c
        src/mem/alloc_log.c
        src/mem/alloc_pool.c
        src/mem/ptr.c
)

set(NX_RAND_SOURCES
        src/rand/rand.c
)

set(NX_NUMERIC_SOURCES
        src/numeric/abs.c
        src/numeric/bit.c
        src/numeric/ckd.c
        src/numeric/fp.c
        src/numeric/sat.c
        src/numeric/sign.c
        src/numeric/bounds.c
)

set(NX_IO_SOURCES
        src/io/dbg.c
        src/io/print.c
)

set(NX_SOURCES
        ${NX_CORE_SOURCES}
        ${NX_DS_SOURCES}
        ${NX_ALGO_SOURCES}
        ${NX_STRING_SOURCES}
        ${NX_MEM_SOURCES}
        ${NX_RAND_SOURCES}
        ${NX_NUMERIC_SOURCES}
        ${NX_IO_SOURCES}
)

add_library(nx_api INTERFACE)
target_include_directories(nx_api INTERFACE ${PROJECT_SOURCE_DIR}/include)
target_compile_features(nx_api INTERFACE c_std_23)
target_link_libraries(nx_api INTERFACE m)

add_library(nx_obj OBJECT ${NX_SOURCES})
target_link_libraries(nx_obj PRIVATE nx_api)
target_compile_options(nx_obj PRIVATE -Wall -Wextra -pedantic)
set_target_properties(nx_obj PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        C_EXTENSIONS OFF
)

add_library(nx_static STATIC $<TARGET_OBJECTS:nx_obj>)
target_link_libraries(nx_static PUBLIC nx_api)

add_library(nx_shared SHARED $<TARGET_OBJECTS:nx_obj>)
target_link_libraries(nx_shared PUBLIC nx_api)

option(NX_BUILD_TEST "Build unit tests" ON)
option(NX_BUILD_BENCH "Build bench test" ON)
option(NX_BUILD_DEMO "Build demos" ON)

if (NX_BUILD_TEST)
    set(UNITY_DIR ${PROJECT_SOURCE_DIR}/thirdparty/Unity-2.6.1)
    add_subdirectory(${UNITY_DIR})
    target_compile_definitions(unity PUBLIC UNITY_INCLUDE_DOUBLE UNITY_INCLUDE_FLOAT)

    enable_testing()
    add_subdirectory(test)
endif ()

if (NX_BUILD_BENCH)
    set(UBENCH_DIR ${PROJECT_SOURCE_DIR}/thirdparty/ubench)
    add_subdirectory(bench)
endif ()

if (NX_BUILD_DEMO)
    set(DEMOS_DIR demo)
    add_subdirectory(${DEMOS_DIR})
endif ()