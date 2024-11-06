add_executable(TestInstantiatorSimpleList   test_simple_list.c)
target_link_libraries(TestInstantiatorSimpleList simple_list)

add_test(NAME TestInstantiatorSimpleList
    COMMAND TestInstantiatorSimpleList)
