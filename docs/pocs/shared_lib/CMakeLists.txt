cmake_minimum_required(VERSION 3.15)

project(
  shared_poc
  VERSION 1.0
  LANGUAGES C
  DESCRIPTION "Register  callbacks client code does not know anything about"
  )

add_executable(main 
  main.c
  callback_lib.c
  callback_lib.h
)