add_library(frixia_utils
    src/simple_queue.c
    src/threadsafe_simple_queue.c
)

target_include_directories(frixia_utils
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)