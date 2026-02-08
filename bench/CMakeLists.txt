function(nx_add_ubench_bench target_name source_file)
    add_executable(${target_name} ${source_file})

    target_include_directories(${target_name} PRIVATE ${UBENCH_DIR})
    target_link_libraries(${target_name} PRIVATE nx_static)
    target_compile_options(${target_name} PRIVATE -Wall -Wextra -pedantic)

    add_test(NAME ${target_name} COMMAND ${target_name})
endfunction()

add_subdirectory(data_structure)