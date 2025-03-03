add_library(timer_callback
    frixia_timer_callback.c
    frixia_timer_callback.h
)

target_link_libraries(timer_callback
    fevent
)