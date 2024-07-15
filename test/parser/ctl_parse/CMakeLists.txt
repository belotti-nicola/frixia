file(COPY test_strings DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(TestInstantiatorParser test_ctl_commands.c)
target_link_libraries(TestInstantiatorParser core)
add_test(NAME TestInstantiatorParser
    COMMAND TestInstantiatorParser)