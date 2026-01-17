cmake_minimum_required(VERSION 3.9)

project(frixia VERSION 1.0.0 DESCRIPTION "Engine for filedescriptors.")

add_subdirectory(src/utils)
add_subdirectory(src/fshinsu_senju/shinsu_senju)
add_subdirectory(src/fdispatcher/dispatcher)

set(FRIXIA_SOURCES
    frixia.c
    src/convoy/convoy.c
    src/fevent/frixia_event.c
    src/fevent/frixia_events_queue.c
    src/fdispatcher/frixia_dispatcher_th.c
    src/fshinsu_senju/frixia_shinsu_senju_th.c
    src/fepoll/frixia_epoll_th.c
)

add_library(frixia
    ${FRIXIA_SOURCES}
)

target_include_directories(frixia
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/internal
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/include
)

target_link_libraries(frixia
    PUBLIC shinsu_senju
    PUBLIC fdispatcher
    PUBLIC fepoll
    PUBLIC frixia_utils
)
