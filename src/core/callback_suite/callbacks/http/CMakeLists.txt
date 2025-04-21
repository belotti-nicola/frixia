add_library(http_callback
    frixia_http_callback.c
    frixia_http_callback.h
)

target_link_libraries(http_callback
    fevent
    callback_data_structure
)