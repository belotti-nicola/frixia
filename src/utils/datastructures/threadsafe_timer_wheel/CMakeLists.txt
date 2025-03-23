add_library(threadsafe_simple_timer_wheel
    ts_timer_wheel.c
    ts_timer_wheel.h
)


target_link_libraries(threadsafe_simple_timer_wheel
    simple_timer_wheel
)