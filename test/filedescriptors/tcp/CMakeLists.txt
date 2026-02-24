add_executable(TestInstantiatorTcp test_tcp.c)
target_link_libraries(TestInstantiatorTcp frixia)
add_test(NAME TestInstantiatorTcp
    COMMAND TestInstantiatorTcp)

set_tests_properties(TestInstantiatorTcp
    PROPERTIES TIMEOUT 20)
