cmake_minimum_required(VERSION 3.28)
project(jhandler-sample)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/jhandler/include)
add_subdirectory(jhandler)

add_executable(
        JHandler-Sample
        sample/main.cpp
        sample/common_use/common_use.cpp
        sample/common_use/message_type.h
        sample/common_use/first_common_use_handler.cpp
        sample/common_use/second_common_use_handler.cpp
        sample/thread_use/thread_use.cpp
        sample/thread_use/gl_thread.cpp
        sample/thread_use/gl_handler.cpp
        sample/thread_use/business_handler.cpp
        sample/log/log.h
)
target_link_libraries(JHandler-Sample PUBLIC jhandler)