function(nx_add_criterion_test target_name source_file)
    add_executable(${target_name} ${source_file})

    target_link_libraries(${target_name} PRIVATE nx_static unity::framework)
    target_compile_options(${target_name} PRIVATE -Wall -Wextra -pedantic)

    add_test(NAME ${target_name} COMMAND ${target_name})
endfunction()

add_subdirectory(algo)
add_subdirectory(core)
add_subdirectory(data_structure)
