add_library(no_protocol_callback
    no_protocol_callback.c
    no_protocol_callback.h
)

target_link_libraries(no_protocol_callback
    event
)