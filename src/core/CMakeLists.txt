add_library(core
  frixia.c
  frixia.h
  ../tcp/frixia_tcp.h
  ../tcp/frixia_tcp.c
  ../udp/frixia_udp.h
  ../udp/frixia_udp.c
)

add_subdirectory(ctl_parser)

target_link_libraries(core 
  ctl_parser
)