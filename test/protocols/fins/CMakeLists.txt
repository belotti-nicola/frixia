function(add_frixia_test test_name source_file)
    add_executable(${test_name} ${source_file})

    # Trova libcurl (fallisce se non c'è)
    find_package(CURL REQUIRED)

    target_link_libraries(${test_name}
        PRIVATE
            frixia
            CURL::libcurl
            ${ARGN}
    )

    add_test(NAME ${test_name}
             COMMAND ${test_name})

    set_tests_properties(${test_name}
        PROPERTIES TIMEOUT 10)
endfunction()


add_frixia_test(TestInstantiatorFINS test_fins.c)
