add_library(callback_data_structure
    callback_data.h
    callback_data.c
    frixia_callbacks.h
    frixia_callbacks.c
    frixia_callback_entry.c
    frixia_callback_entry.h
    )

target_link_libraries(callback_data_structure
    simple_list
    simple_hash_map
)
