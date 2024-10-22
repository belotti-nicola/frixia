add_subdirectory(proto_filedescriptor)
add_subdirectory(proto_callbacks)

add_library(fsetup
    setup_utility.c
    setup_utility.h
)

target_link_libraries(fsetup
    proto_callbacks
    proto_filedescriptor
    core
)
