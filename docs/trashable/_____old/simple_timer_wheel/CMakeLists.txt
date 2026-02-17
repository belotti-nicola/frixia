add_executable(TestSimpleWheelTimer
    test_simple_timer_wheel.c)

target_link_libraries(TestSimpleWheelTimer 
    simple_timer_wheel)

add_test(NAME TestSimpleWheelTimer
    COMMAND TestSimpleWheelTimer)
