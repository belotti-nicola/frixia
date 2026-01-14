add_library(frixia_utils
    src/simple_queue.c
    src/threadsafe_simple_queue.c
    src/simple_valid_callback.c
    src/simple_list_elem.c
    src/simple_queue.c
    src/simple_hash_entry.c
    src/simple_hash_map.c
    src/simple_hashing_utility.c
)

target_include_directories(frixia_utils
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)