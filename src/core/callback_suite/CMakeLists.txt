add_subdirectory(hashmap)
add_subdirectory(callback_data)


add_library(callback_suite
    frixia_cb_hashmap.c
    frixia_cb_hashmap.h
    )

target_link_libraries(callback_suite
    hashmap
    callback_data_structure
)