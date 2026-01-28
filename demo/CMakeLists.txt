function(nx_add_demo target_name source_file)
    add_executable(${target_name} ${source_file})
    target_link_libraries(${target_name} PRIVATE c_naive_extension_static)
endfunction()

add_subdirectory(data_structure)