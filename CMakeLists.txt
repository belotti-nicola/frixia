cmake_minimum_required(VERSION 3.15)

project(frixia C)
include(CTest)

find_package(Threads REQUIRED)

set(CMAKE_C_FLAGS -pthread)

add_subdirectory(deps)
add_subdirectory(src)
add_subdirectory(test)

add_executable(frixia
  main.c
)

target_link_libraries(frixia 
  core
  picohttpparser
  threadsafe_simple_queue
  simple_queue
)

