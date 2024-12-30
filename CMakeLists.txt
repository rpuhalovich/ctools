cmake_minimum_required(VERSION 3.28.3)
project(ctools C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra -Wpedantic -Werror")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

add_executable(ctemplate ${CMAKE_SOURCE_DIR}/src/ctemplate.c)
add_executable(cenum ${CMAKE_SOURCE_DIR}/src/cenum.c)
