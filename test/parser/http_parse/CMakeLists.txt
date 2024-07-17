file(COPY test_strings DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(TestHTTPParser test_http_strings.c)
target_link_libraries(TestHTTPParser core)
add_test(NAME TestHTTPParser
    COMMAND TestHTTPParser)