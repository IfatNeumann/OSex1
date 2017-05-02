cmake_minimum_required(VERSION 3.6)
project(OSex1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES ex11.c)
add_executable(OSex1 ${SOURCE_FILES})