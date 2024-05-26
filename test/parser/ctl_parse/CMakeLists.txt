add_executable(TestInstantiator test_ctl_commands.c)
target_link_libraries(TestInstantiator ctl_parser)
add_test(NAME TestInstantiator
    COMMAND TestInstantiator)

add_executable(TestInstantiator2 test_ctl_commands2.c)
target_link_libraries(TestInstantiator2 ctl_parser)
add_test(NAME TestInstantiator2
    COMMAND TestInstantiator2)
