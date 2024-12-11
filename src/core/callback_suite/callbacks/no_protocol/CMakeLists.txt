add_library(no_protocol_callback
    frixia_no_protocol_callback.c
    frixia_no_protocol_callback.h
)

target_link_libraries(no_protocol_callback
    fevent
)