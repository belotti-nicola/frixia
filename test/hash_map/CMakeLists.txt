add_executable(TestInstantiatorHashMapStrings   test_hash_map_strings.c)
add_executable(TestInstantiatorHashMapFunctions test_hash_map_functions.c)
target_link_libraries(TestInstantiatorHashMapStrings core)
target_link_libraries(TestInstantiatorHashMapFunctions core)

add_test(NAME TestInstantiatorHashMapStrings
    COMMAND TestInstantiatorHashMapStrings)
add_test(NAME TestInstantiatorHashMapFunctions
    COMMAND TestInstantiatorHashMapFunctions)