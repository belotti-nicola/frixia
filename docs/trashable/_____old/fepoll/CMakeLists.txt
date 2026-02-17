add_executable(TestInstantiatorFepoll test_fepoll.c)
target_link_libraries(TestInstantiatorFepoll fepoll)
add_test(NAME TestInstantiatorFepoll
    COMMAND TestInstantiatorFepoll)