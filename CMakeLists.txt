cmake_minimum_required(VERSION 3.28)
project(JHandler-Demo)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/JHandler/include)
add_subdirectory(JHandler)

add_executable(
        JHandler-Demo
        sample/main.cpp
        sample/simpleUse/SimpleUse.cpp
)

target_link_libraries(JHandler-Demo PUBLIC JHandler)