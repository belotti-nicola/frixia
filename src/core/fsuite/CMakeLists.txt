add_library(fsuite
  frixia_fd.c
  frixia_fd.h
  frixia_suite.h
  frixia_suite.c 
)

target_link_libraries(fsuite
  simple_list
)