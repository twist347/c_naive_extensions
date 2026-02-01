function(nx_add_demo target_name source_file)
    add_executable(${target_name} ${source_file})
    target_link_libraries(${target_name} PRIVATE nx_static)
    target_compile_options(${target_name} PRIVATE -Wall -Wextra -pedantic)
endfunction()

add_subdirectory(data_structure)
add_subdirectory(str)