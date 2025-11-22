cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(
        jhandler
        SHARED
        src/handler.cpp
        src/handler_thread.cpp
        src/looper.cpp
        src/message.cpp
        src/message_queue.cpp
        src/log.cpp
)