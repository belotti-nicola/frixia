cmake_minimum_required(VERSION 3.15)

project(
  return_mappings_poc
  LANGUAGES C
  DESCRIPTION "select and run one of two functions with return type that is a enum"
  )

add_executable(main 
  main.c
  foo_function.h
  foo_function.c
  moo_function.h
  moo_function.c
  return_type.h
)