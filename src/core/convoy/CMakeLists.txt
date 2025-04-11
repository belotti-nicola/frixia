add_library(convoy
    convoy.h
    convoy.c
    frixia_fd_args.c
    frixia_fd_args.c
    fd.h
    fd.c
    frixia_callback.c
    frixia_callback.h
)

target_link_libraries(convoy
    simple_hash_map
)