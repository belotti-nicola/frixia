cmake_minimum_required(VERSION 3.15)

project(frixia C)


add_subdirectory(src)

add_executable(frixia
  main.c
)

target_link_libraries(frixia 
  core
)
