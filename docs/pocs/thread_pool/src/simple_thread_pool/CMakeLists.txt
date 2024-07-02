add_library(simple_thread_pool
    sthread_pool.c
    sthread_pool.h
    )

target_link_libraries(simple_thread_pool
    Threads::Threads
    simple_queue
    )