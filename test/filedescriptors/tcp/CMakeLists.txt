add_executable(TestInstantiatorTcp test_tcp.c)
target_link_libraries(TestInstantiatorTcp frixia)
add_test(NAME TestInstantiatorFepoll
    COMMAND TestInstantiatorTcp)