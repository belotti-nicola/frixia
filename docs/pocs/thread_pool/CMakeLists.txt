cmake_minimum_required(VERSION 3.15)

project(thread_pool_poc C)

find_package(Threads REQUIRED)

add_subdirectory(src)

add_executable(thread_pool_poc
  main.c
)

target_link_libraries(thread_pool_poc 
  simple_queue
  simple_thread_pool
)

