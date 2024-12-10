add_subdirectory(http)
add_subdirectory(fins)

add_library(protocols
    frixia_supported_protocols.h
    frixia_supported_protocols.c
    )

target_link_libraries(protocols 
    http
    fins
  )