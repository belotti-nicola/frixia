add_library(fevent
  frixia_event.h
  frixia_event.c
  frixia_events_queue.c
  frixia_events_queue.h
)

target_link_libraries(fevent
  threadsafe_simple_queue
)