cmake_minimum_required(VERSION 4.1.2)
set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)

project(md2html CXX)

include_directories(./include)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(./src SOURCES)

message("Source Files: " ${SOURCES})

add_executable(md2html ${SOURCES})