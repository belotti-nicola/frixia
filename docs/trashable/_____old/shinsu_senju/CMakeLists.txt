add_executable(TestInstantiatorShinsuSenju test_shinsu_senju.c)

target_link_libraries(TestInstantiatorShinsuSenju
    shinsu_senju
)

add_test(NAME TestInstantiatorShinsuSenju
    COMMAND TestInstantiatorShinsuSenju)