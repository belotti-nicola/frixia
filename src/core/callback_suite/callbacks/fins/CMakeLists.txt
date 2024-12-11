add_library(fins_callback
    frixia_fins_callback.h
    frixia_fins_callback.c
)

target_link_libraries(no_protocol_callback
    fevent
)