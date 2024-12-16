# JHandler

JHandler 是一套 C++ 的事件轮询机制。可在自行创建的线程中使用，也可以使用 JHandler 已封装好的独立线程处理事件。

## 集成

添加 `JHandler` 目录到项目中即可，在项目的 `CMakeLists.txt` 中添加以下配置即可：

```cmake
include_directories(“添加的 JHandler 目录相对于当前 CMakeLists.txt 文件的路径”/JHandler/include)
add_subdirectory(JHandler)

target_link_libraries(“链接目标名称” PUBLIC JHandler)
```

## 如何使用？

可以参考 `sample` 目录中的 `simpleUse`（常规使用） 和 `diyThread`（自定义线程） 方法。

### 常规使用

`JHandler` 中已经准备好了 `HandlerThread` ，内部会创建独立线程，并启动事件轮询。通过 `Handler` 放入的事件句

```cpp
```
