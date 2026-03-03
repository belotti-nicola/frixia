add_library(picohttpparser
    picohttpparser/picohttpparser.h
    picohttpparser/picohttpparser.c
    )

target_include_directories(picohttpparser
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)