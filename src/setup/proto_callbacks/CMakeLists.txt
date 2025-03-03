add_subdirectory(pc_http)
add_subdirectory(pc_noprotocol)
add_subdirectory(pc_timer)

add_library( proto_callbacks
    proto_cb.h
    proto_cb.c
    proto_callbacks_queue.h
    proto_callbacks_queue.c
)

target_link_libraries(proto_callbacks
    fqueue
    protocols
    proto_callbacks_http
    proto_callbacks_noprotocol
    proto_callbacks_timer
    )