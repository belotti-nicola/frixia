add_library(core
  frixia.c
  frixia.h
  frixia_common.h
  frixia_common.c 
)

add_subdirectory(ctl_parser)
add_subdirectory(fd_pool)
add_subdirectory(fqueue)
add_subdirectory(thread_pool)
add_subdirectory(fevent)
add_subdirectory(protocols)
add_subdirectory(callback_suite)
add_subdirectory(filedescriptor)


target_link_libraries(core 
  ctl_parser
  fd_pool
  fqueue
  thread_pool
  fevent
  proto_callbacks
  proto_filedescriptor
  protocols
  tcp
  fifo
  udp
  callback_suite
  picohttpparser
  frixia_monitor
  fsetup
)