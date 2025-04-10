add_library(bound_robin
    bound_robin.h
    bound_robin.c
    bound_robin_thread.c
    bound_robin_thread.h
    bound_robin_event.h
    bound_robin_event.c
    detached_bound_robin.h
    detached_bound_robin.h
)

target_link_libraries(bound_robin
    threadsafe_simple_queue
)