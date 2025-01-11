cmake_minimum_required(VERSION 3.28.3)
project(ctools C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

set(FILE_SRC ${CMAKE_SOURCE_DIR}/src/file.c ${CMAKE_SOURCE_DIR}/src/file.h)

add_executable(ctemplate ${FILE_SRC} ${CMAKE_SOURCE_DIR}/src/ctemplate.c)
target_include_directories(ctemplate PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_executable(cenum ${FILE_SRC} ${CMAKE_SOURCE_DIR}/src/cenum.c)
target_include_directories(cenum PUBLIC ${CMAKE_SOURCE_DIR}/src)
