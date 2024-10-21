add_library(http
    frixia_http_parser.c
    frixia_http_parser.h
    )

target_link_libraries(http
    picohttpparser
)