add_library(setup
  proto_fds_queue.h
  proto_fds_queue.c
  proto_fds.c
  proto_fds.h
  ../../frixia_common.h
  ../../frixia_common.c
  ../../fqueue/frixia_queue.c
  ../../fqueue/frixia_queue.h
  ../../fd_pool/filedescriptor_pool_defs.h
  ../../fd_pool/filedescriptor_pool_defs.c
)

target_link_libraries(setup
  protocols
)