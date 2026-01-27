cmake_minimum_required(VERSION 3.20)

project(c_naive_extension)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(NX_CORE_SOURCES
        src/core/cmp.c
        src/core/print.c
        src/core/span.c
        src/core/status.c
        src/core/checked_arith.c
)

set(NX_DS_SOURCES
        src/data_structure/arr.c
        src/data_structure/vec.c
)

set(NX_ALGO_SOURCES
        src/algo/algo.c
)

set(NX_STRING_SOURCES
        src/str/str.c
        src/str/str_view.c
)

set(NX_MEM_SOURCES
        src/mem/byte.c
)

set(NX_SOURCES
        ${NX_CORE_SOURCES}
        ${NX_DS_SOURCES}
        ${NX_ALGO_SOURCES}
        ${NX_STRING_SOURCES}
        ${NX_MEM_SOURCES}
)

add_library(c_naive_extension_static STATIC
        ${NX_SOURCES}
)

add_library(c_naive_extension_shared SHARED
        ${NX_SOURCES}
)

set_target_properties(c_naive_extension_static PROPERTIES
        OUTPUT_NAME c_naive_extension
)

set_target_properties(c_naive_extension_shared PROPERTIES
        OUTPUT_NAME c_naive_extension
)

foreach(tgt IN ITEMS c_naive_extension_static c_naive_extension_shared)
    target_include_directories(${tgt} PUBLIC ${CMAKE_SOURCE_DIR}/include)
    target_compile_options(${tgt} PRIVATE -Wall -Wextra -pedantic)
    target_link_libraries(${tgt} PUBLIC m)
endforeach()

option(BUILD_TESTS "Build unit tests" ON)

if (BUILD_TESTS)
    set(CRITERION_DIR thirdparty/criterion-2.4.3)
    enable_testing()
    add_subdirectory(test)
endif ()