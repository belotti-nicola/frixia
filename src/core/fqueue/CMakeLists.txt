add_library(fqueue
  frixia_queue.c
  frixia_queue.h
)

target_link_libraries(fqueue
  Threads::Threads
)