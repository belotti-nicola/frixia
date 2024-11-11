add_executable(TestInstantiatorSimpleListSizes           test_simple_list_sizes.c)
add_executable(TestInstantiatorSimpleListPrevious        test_simple_list_previous.c)
add_executable(TestInstantiatorSimpleListNext            test_simple_list_next.c)
target_link_libraries(TestInstantiatorSimpleListSizes    simple_list)
target_link_libraries(TestInstantiatorSimpleListPrevious simple_list)
target_link_libraries(TestInstantiatorSimpleListNext     simple_list)

add_test(NAME TestInstantiatorSimpleListSizes
    COMMAND TestInstantiatorSimpleListSizes)
add_test(NAME TestInstantiatorSimpleListPrevious
    COMMAND TestInstantiatorSimpleListPrevious)
add_test(NAME TestInstantiatorSimpleListNext
    COMMAND TestInstantiatorSimpleListNext)
