cmake_minimum_required(VERSION 3.28)
project(jhandler-sample)

set(CMAKE_CXX_STANDARD 17)

# 添加头文件搜索路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/jhandler/include)

# 【链接本地】子目录添加到构建系统中
add_subdirectory(jhandler)
# 【链接编译后的 lib】添加库文件搜索路径
#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_executable(
        JHandler-Sample
        sample/main.cpp
#        sample/JHandlerDemo.cpp
        sample/simpleUse/SimpleUse.cpp
        sample/diyThread/DiyThread.cpp
)
target_link_libraries(JHandler-Sample PUBLIC jhandler)