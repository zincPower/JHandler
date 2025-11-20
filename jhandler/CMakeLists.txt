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

## 编译
## mkdir build
## cd build
## cmake ..
## make
#set_target_properties(
#        jhandler PROPERTIES
#        VERSION 1.0.0   # 完整版本号
#        SOVERSION 1     # ABI 版本号
#)

# 针对不同平台设置不同的输出
if (ANDROID)
    set_target_properties(jhandler PROPERTIES
            VERSION 1.0.0
            SOVERSION 1
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/android/${ANDROID_ABI}"
    )
elseif (HARMONYOS)
    set_target_properties(jhandler PROPERTIES
            VERSION 1.0.0
            SOVERSION 1
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/harmony/${OHOS_ARCH}"
    )
else ()
    set_target_properties(jhandler PROPERTIES
            VERSION 1.0.0
            SOVERSION 1
            LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/macos"
    )
endif ()