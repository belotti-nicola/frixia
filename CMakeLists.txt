cmake_minimum_required(VERSION 3.9)

project(frixia VERSION 1.0.0 DESCRIPTION "Engine for filedescriptors.")

find_package(Threads REQUIRED)

set(FRIXIA_SOURCES
    frixia.c
    src/convoy/convoy.c
    src/convoy/fd_args.c
    src/fevent/frixia_event.c
    src/fevent/frixia_events_queue.c
    src/fdispatcher/frixia_dispatcher_th.c
    src/fdispatcher/dispatcher/detached_frixia_dispatcher_new.h
    src/fdispatcher/dispatcher/detached_frixia_dispatcher_new.c
    src/fdispatcher/dispatcher/frixia_dispatcher_loop_function.h
    src/fdispatcher/dispatcher/frixia_dispatcher_loop_function.c
    src/fdispatcher/dispatcher/frixia_dispatcher.h
    src/fdispatcher/dispatcher/frixia_dispatcher.c
    src/fshinsu_senju/frixia_shinsu_senju_th.c
    src/fshinsu_senju/shinsu_senju/shinsu_senju_loop_function.h
    src/fshinsu_senju/shinsu_senju/shinsu_senju_loop_function.c
    src/fshinsu_senju/shinsu_senju/ss_pool.c
    src/fshinsu_senju/shinsu_senju/ss_wrapper_function.c
    src/fshinsu_senju/shinsu_senju/shinsu_senju_loop_function.c
    src/fepoll/frixia_epoll_th.c
    src/fepoll/epoll/epoll.h
    src/fepoll/epoll/epoll.c
    src/fepoll/epoll/fepoll_codes.h
    src/fepoll/epoll/fepoll_loop_function.h
    src/fepoll/epoll/fepoll_loop_function.c
    src/fepoll/epoll/fepoll_pool_data.h
    src/fepoll/epoll/fepoll_pool_data.c
    src/fepoll/epoll/fepoll_pool.h
    src/fepoll/epoll/fepoll_pool.c
    src/fepoll/epoll/fepoll.c
    src/fepoll/epoll/fepoll.h
    src/fepoll/epoll/fctx.h
    src/fepoll/epoll/fctx.c
    src/fepoll/epoll/ftcp_handler.h
    src/fepoll/epoll/ftcp_handler.c
    src/fepoll/epoll/fudp_handler.h
    src/fepoll/epoll/fudp_handler.c
    src/fepoll/epoll/ffifo_handler.h
    src/fepoll/epoll/ffifo_handler.c
    src/fepoll/epoll/feventfd_handler.h
    src/fepoll/epoll/feventfd_handler.c
    src/fepoll/epoll/ftimer_handler.h
    src/fepoll/epoll/ftimer_handler.c
    src/fepoll/epoll/fsignal_handler.h
    src/fepoll/epoll/fsignal_handler.c
    src/fepoll/epoll/finode_handler.h
    src/fepoll/epoll/finode_handler.c  
    src/fepoll/epoll/event_context.h
    src/fepoll/epoll/event_context.c

    src/utils/network.c
    src/utils/simple_hash_entry.c
    src/utils/simple_hash_map.c
    src/utils/simple_hashing_utility.c
    src/utils/simple_hashing_utility.h
    src/utils/simple_list_elem.c
    src/utils/simple_list.c
    src/utils/simple_queue.c
    src/utils/simple_valid_callback.c
    src/utils/threadsafe_simple_queue.c
)

add_library(frixia
    ${FRIXIA_SOURCES}
)

target_include_directories(frixia
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/internal
)

target_link_libraries(frixia
    PRIVATE
        Threads::Threads
)