add_executable(TestInstantiatorTcp test_tcp.c)
target_link_libraries(TestInstantiatorTcp frixia)
add_test(NAME TestInstantiatorTcp
    COMMAND TestInstantiatorTcp)

set_tests_properties(TestInstantiatorTcp
    PROPERTIES TIMEOUT 5)

add_executable(TestInstantiatorTermination test_termination.c)
target_link_libraries(TestInstantiatorTermination frixia)
add_test(NAME TestInstantiatorTermination
    COMMAND TestInstantiatorTermination)

set_tests_properties(TestInstantiatorTermination
    PROPERTIES TIMEOUT 5)

