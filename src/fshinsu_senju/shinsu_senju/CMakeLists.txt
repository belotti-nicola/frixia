add_library(shinsu_senju
  ss_pool.c
  ss_pool.h
  ss_wrapper_function.h
  ss_wrapper_function.c
)

target_link_libraries(shinsu_senju
  PUBLIC fevent
  PUBLIC frixia_utils
  PUBLIC frixia
)