cmake_minimum_required(VERSION 3.28.3)
project(ctools C)
set(CMAKE_C_STANDARD 17)

add_executable(ctemplate ${CMAKE_SOURCE_DIR}/src/ctemplate.c)
add_executable(cenum ${CMAKE_SOURCE_DIR}/src/cenum.c)
add_executable(cdocs ${CMAKE_SOURCE_DIR}/src/cdocs.c)
