function(add_frixia_test test_name source_file)
    add_executable(${test_name} ${source_file})

    target_link_libraries(${test_name}
        PRIVATE
            frixia
            ${ARGN}
    )

    add_test(NAME ${test_name}
             COMMAND ${test_name})

    set_tests_properties(${test_name}
        PROPERTIES TIMEOUT 10)
endfunction()

add_frixia_test(TestInstantiatorTcp         test_tcp.c)
add_frixia_test(TestInstantiatorTermination test_termination.c)
add_frixia_test(TestInstantiatorRead        test_read.c)
add_frixia_test(TestInstantiatorClose       test_close.c)

add_frixia_test(TestInstantiatorHttp        test_http.c)


