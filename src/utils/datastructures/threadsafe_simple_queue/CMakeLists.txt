add_library(threadsafe_simple_queue
    threadsafe_simple_queue.c
    threadsafe_simple_queue.h
)

target_link_libraries(threadsafe_simple_queue
    PUBLIC simple_queue
)