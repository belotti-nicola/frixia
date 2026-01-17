cmake_minimum_required(VERSION 3.15)
project(c_poc LANGUAGES C)

find_package(Threads REQUIRED)

add_executable(c_poc
    main.c
)

# Crea un target IMPORTED per la libreria .a
add_library(frixia STATIC IMPORTED)

# Dove si trova il file .a
set_target_properties(frixia PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/../../../build/libfrixia.a"
)

# Header pubblici della libreria
target_include_directories(frixia INTERFACE
    "${CMAKE_SOURCE_DIR}/../../../include"
)

target_link_libraries(c_poc
    PRIVATE
        frixia
        Threads::Threads
)