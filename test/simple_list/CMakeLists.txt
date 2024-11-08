add_executable(TestInstantiatorSimpleList           test_simple_list.c)
add_executable(TestInstantiatorSimpleListNext       test_simple_list_next.c)
add_executable(TestInstantiatorSimpleListPrevious   test_simple_list_previous.c)
target_link_libraries(TestInstantiatorSimpleList            simple_list)
target_link_libraries(TestInstantiatorSimpleListNext        simple_list)
target_link_libraries(TestInstantiatorSimpleListPrevious    simple_list)

add_test(NAME TestInstantiatorSimpleListNext
    COMMAND TestInstantiatorSimpleListNext)
