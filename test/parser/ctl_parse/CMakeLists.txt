file(COPY test_strings DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(TestInstantiator test_ctl_commands.c)
target_link_libraries(TestInstantiator ctl_parser)
add_test(NAME TestInstantiator
    COMMAND TestInstantiator)