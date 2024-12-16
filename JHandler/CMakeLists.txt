cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(
        jhandler
        SHARED
        src/Handler.cpp
        src/HandlerThread.cpp
        src/Looper.cpp
        src/Message.cpp
        src/MessageQueue.cpp
        src/Log.cpp
)

# 编译
# mkdir build
# cd build
# cmake ..
# make
set_target_properties(
        jhandler PROPERTIES
        VERSION 1.0.0   # 完整版本号
        SOVERSION 1     # ABI 版本号
)