add_library(core
  frixia.c
  frixia.h
  frixia_common.h
  frixia_common.c
  ../tcp/frixia_tcp.h
  ../tcp/frixia_tcp.c
  ../udp/frixia_udp.h
  ../udp/frixia_udp.c
  ../fifo/frixia_fifo.h
  ../fifo/frixia_fifo.c
  
)

add_subdirectory(ctl_parser)
add_subdirectory(fd_pool)


target_link_libraries(core 
  ctl_parser
  fd_pool
)