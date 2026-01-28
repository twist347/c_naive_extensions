function(nx_add_example target_name source_file)
    add_executable(${target_name} ${source_file})
    target_link_libraries(${target_name} PRIVATE c_naive_extension_static)
endfunction()

nx_add_example(nx_arr data_structure/arr.c)