add_library(crono
    crono.c
    crono.h
)

target_link_libraries(crono
    threadsafe_simple_timer_wheel
)