cmake_minimum_required(VERSION 3.20)

project(c_naive_extension)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(NX_CORE_SOURCES
        src/core/cmp.c
        src/core/print.c
        src/core/span.c
        src/core/status.c
        src/core/hash.c
)

set(NX_DS_SOURCES
        src/data_structure/arr.c
        src/data_structure/vec.c
)

set(NX_ALGO_SOURCES
        src/algo/algo.c
)

set(NX_STR_SOURCES
        src/str/str.c
        src/str/str_view.c
)

set(NX_MEM_SOURCES
        src/mem/ptr.c
)

set(NX_RAND_SOURCES
        src/rand/rand.c
)

set(NX_SOURCES
        ${NX_CORE_SOURCES}
        ${NX_DS_SOURCES}
        ${NX_ALGO_SOURCES}
        ${NX_STR_SOURCES}
        ${NX_MEM_SOURCES}
        ${NX_RAND_SOURCES}
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

foreach (tgt IN ITEMS nx_static nx_shared)
    target_include_directories(${tgt} PUBLIC ${CMAKE_SOURCE_DIR}/include)
    target_compile_options(${tgt} PRIVATE -Wall -Wextra -pedantic)
    target_link_libraries(${tgt} PUBLIC m)
endforeach ()

option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_DEMOS "Build demos" ON)

if (BUILD_TESTS)
    set(CRITERION_DIR thirdparty/criterion-2.4.3)
    enable_testing()
    add_subdirectory(test)
endif ()

if (BUILD_DEMOS)
    set(DEMOS_DIR demo)
    add_subdirectory(${DEMOS_DIR})
endif ()