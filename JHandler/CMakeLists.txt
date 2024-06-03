cmake_minimum_required(VERSION 3.28)
project(JHandler)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(
        JHandler
        src/Handler.cpp
        src/HandlerThread.cpp
        src/Looper.cpp
        src/Message.cpp
        src/MessageQueue.cpp
        src/Log.cpp
)
