add_library(callback_data_structure
    hashed_callback.h
    hashed_callback.c
    callback_data.h
    callback_data.c
    frixia_callbacks.h
    frixia_callbacks.c
    )

target_link_libraries(callback_data_structure
    hashmap
    simple_list
)
