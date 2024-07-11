add_subdirectory(hashmap)

add_library(callback_suite
    frixia_cb_hashmap.c
    frixia_cb_hashmap.h
    frixia_cb_data.h
    frixia_cb_data.c
    )

target_link_libraries(callback_suite
    hashmap
)