add_executable(TestInstantiatorInodes test_finodes.c)
target_link_libraries(TestInstantiatorInodes core)
add_test(NAME TestInstantiatorInodes
    COMMAND TestInstantiatorInodes)