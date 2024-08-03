add_library(callback_data_structure
    callback_data_structure.h
    callback_data_structure.c
    )

target_link_libraries(callback_data_structure
    hashmap
)
