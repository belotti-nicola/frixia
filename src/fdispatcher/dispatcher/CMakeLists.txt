add_library(fdispatcher
    frixia_dispatcher.c
    frixia_dispatcher.h
    frixia_dispatcher_loop_function.c
    frixia_dispatcher_loop_function.h
    detached_frixia_dispatcher_new.c
    detached_frixia_dispatcher_new.h
)

target_link_libraries(fdispatcher
  PUBLIC fevent
  PUBLIC frixia_utils
  PUBLIC frixia
)