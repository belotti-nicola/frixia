add_library(crono
    crono.c
    crono.h
    detached_start_scheduler.c
    detached_start_scheduler.h
)

target_link_libraries(crono
    threadsafe_simple_timer_wheel
)