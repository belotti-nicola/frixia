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

add_frixia_test(TestInstantiatorSignalTermination test_signal.c)


