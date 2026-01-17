add_library(shinsu_senju
  ss_pool.c
  ss_wrapper_function.c
  shinsu_senju_loop_function.h
  shinsu_senju_loop_function.c
)

target_link_libraries(shinsu_senju
  PUBLIC fevent
  PUBLIC frixia_utils
  PUBLIC frixia
)