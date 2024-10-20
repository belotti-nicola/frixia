add_subdirectory(epoll)

add_library(frixia_monitor
    detached_epoll_monitor.c
    detached_epoll_monitor.h
)

target_link_libraries(frixia_monitor
    Threads::Threads
    fepoll
)