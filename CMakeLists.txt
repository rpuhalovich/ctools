cmake_minimum_required(VERSION 3.28.3)
project(ctools C)
set(CMAKE_C_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_executable(ctemplate ${CMAKE_SOURCE_DIR}/src/ctemplate.c)
add_executable(cenum ${CMAKE_SOURCE_DIR}/src/cenum.c)
