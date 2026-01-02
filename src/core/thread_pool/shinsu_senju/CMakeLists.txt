add_library(shinsu_senju
  ss_pool.c
  ss_pool.h
  detached_shinsu_senju.h
  detached_shinsu_senju.c
  ss_wrapper_function.h
  ss_wrapper_function.c
)

target_link_libraries(shinsu_senju
  fevent
  simple_valid_callback
)