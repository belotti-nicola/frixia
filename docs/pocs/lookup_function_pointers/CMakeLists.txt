cmake_minimum_required(VERSION 3.15)

project(
  main
  VERSION 1.0
  LANGUAGES CXX
  DESCRIPTION "poc of function pointers and look up tables"
  )

add_executable(main 
  main.cpp
  poc_header.h
)
