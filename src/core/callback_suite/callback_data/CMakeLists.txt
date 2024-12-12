add_library(callback_data_structure
    callback_data.h
    callback_data.c
    callbacks_data_structure.h
    callbacks_data_structure.c
    )

target_link_libraries(callback_data_structure
    hashmap
    simple_list
)
