function(nx_add_criterion_test target_name source_file)
    add_executable(${target_name} ${source_file})

    target_include_directories(${target_name} PRIVATE ${CRITERION_DIR}/include)
    target_link_directories(${target_name} PRIVATE ${CRITERION_DIR}/lib)

    target_link_libraries(${target_name} PRIVATE c_naive_extension_static criterion)

    add_test(NAME ${target_name} COMMAND ${target_name})
endfunction()

nx_add_criterion_test(test_nx_arr  test_arr.c)
nx_add_criterion_test(test_nx_algo test_algo.c)
nx_add_criterion_test(test_nx_vec test_vec.c)
nx_add_criterion_test(test_nx_span test_span.c)