add_subdirectory(callback_data)
add_subdirectory(callbacks)


add_library(callback_suite
    frixia_callback_main_function.c
    frixia_callback_main_function.h
    )

target_link_libraries(callback_suite
    callback_data_structure
    http_callback
    fins_callback
    no_protocol_callback
)