add_subdirectory(bound_robin)

add_library(thread_pool
  fthread_pool.c
  fthread_pool.h
)

target_link_libraries(thread_pool
  fqueue
)