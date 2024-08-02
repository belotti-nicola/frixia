add_subdirectory(pc_http)
add_subdirectory(pc_noprotocol)

add_library( proto_callbacks
    proto_cb.c
    proto_cb.h
    proto_callbacks_queue.h
    proto_callbacks_queue.c
)

target_link_libraries(proto_callbacks
    fqueue
    protocols
)