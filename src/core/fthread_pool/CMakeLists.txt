add_library(fthread_pool
    frixia_thread_pool.c
    frixia_thread_pool.h
    frixia_thread_pool_data.c
    frixia_thread_pool_data.h
)

target_link_libraries(fthread_pool
    fd_reader
)