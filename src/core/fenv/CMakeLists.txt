add_library(fenv
    frixia_environment.h
    frixia_environment.c
)

target_link_libraries(fenv
    tcp
    udp
    fevent
    fepoll
    fdispatcher
)
