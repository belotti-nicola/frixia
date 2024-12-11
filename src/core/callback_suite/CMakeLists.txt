add_subdirectory(hashmap)
add_subdirectory(callback_data)
add_subdirectory(callbacks)


add_library(callback_suite
    frixia_callback_main_function.c
    frixia_callback_main_function.h
    frixia_cb_hashmap.c
    frixia_cb_hashmap.h
    )

target_link_libraries(callback_suite
    hashmap
    callback_data_structure
    http_callback
    fins_callback
    no_protocol_callback
)