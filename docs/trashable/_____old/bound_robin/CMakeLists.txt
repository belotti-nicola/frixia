add_executable(TestInstantiatorBoundRobin test_bound_robin.c)
target_link_libraries(TestInstantiatorBoundRobin 
    pthread
    bound_robin
    threadsafe_simple_queue
    simple_queue
)
add_test(NAME TestInstantiatorBoundRobin
    COMMAND TestInstantiatorBoundRobin)