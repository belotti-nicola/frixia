add_library(tcp
  frixia_tcp.c
  frixia_tcp.h
  frixia_tcp_codes.h
  frixia_tcp_fd_result.h
  frixia_tcp_fd_result.c
  frixia_tcp_result.h
  frixia_tcp_result.c
  frixia_tcp_write_result.h
  frixia_tcp_write_result.c
  frixia_tcp_read_result.h
  frixia_tcp_read_result.c
)

target_link_libraries(tcp
  simple_networking
)