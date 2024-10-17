add_library(proto_filedescriptor
  proto_fds_queue.h
  proto_fds_queue.c
  proto_fds.c
  proto_fds.h
)

target_link_libraries(proto_filedescriptor
  protocols
)