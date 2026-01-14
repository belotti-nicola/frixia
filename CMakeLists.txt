cmake_minimum_required(VERSION 3.9)

project(frixia VERSION 1.0.0 DESCRIPTION "Engine for filedescriptors.")

add_subdirectory(src/utils)
#add_subdirectory(src/fepoll/epoll)

set(FRIXIA_SOURCES
    frixia.c
    src/convoy/convoy.c
    src/fevent/frixia_event.c
    src/fevent/frixia_events_queue.c
)

add_library(frixia
    ${FRIXIA_SOURCES}
)

target_include_directories(frixia
    PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/internal
)

target_link_libraries(frixia
    PRIVATE frixia_utils
)
