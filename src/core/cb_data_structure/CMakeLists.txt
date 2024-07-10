add_subdirectory(hashmap)

add_library(cb_data_structure
    frixia_cb_hashmap.c
    frixia_cb_hashmap.h
    frixia_cb_data.h
    frixia_cb_data.c
    )

target_link_libraries(cb_data_structure
    hashmap
)