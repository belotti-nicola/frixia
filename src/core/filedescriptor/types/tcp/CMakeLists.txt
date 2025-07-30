add_library(tcp
  frixia_tcp.c
  frixia_tcp.h
)

target_link_libraries(tcp
  simple_networking
)