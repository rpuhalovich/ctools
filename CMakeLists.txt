cmake_minimum_required(VERSION 3.28.3...3.28.3)
project(ctemplate C)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DRELEASE -O3")

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(EXTERN_DIR "${CMAKE_SOURCE_DIR}/extern")

file(GLOB_RECURSE src ${SRC_DIR}/main.c)
add_executable(${PROJECT_NAME} ${src})
