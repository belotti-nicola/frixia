add_subdirectory(fins)
add_subdirectory(http)

add_library(protocols INTERFACE)

target_link_libraries(protocols
    INTERFACE
        fins
        http
)