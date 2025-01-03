cmake_minimum_required(VERSION 3.15)

project(
  poc_atomic
  LANGUAGES C
  )

find_package(Threads REQUIRED)

add_executable(main 
  main.c
)

target_link_libraries(main
    pthread
)


