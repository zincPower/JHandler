cmake_minimum_required(VERSION 3.28)
project(JHandler-Demo)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(JHandler)

add_executable(
        JHandler-Demo
        main.cpp
)
