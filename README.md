# JHandler

JHandler 是一套 C++ 的事件轮询机制。可在自行创建的线程中使用，也可以使用 JHandler 已封装好的独立线程处理事件。

## 集成

将 `jhandler` 文件夹拷贝添加到项目中，并在项目的 `CMakeLists.txt` 中添加以下配置即可：

```cmake
include_directories(“添加的 jhandler 目录相对于当前 CMakeLists.txt 文件的路径”/JHandler/include)
add_subdirectory(jhandler)

target_link_libraries(“链接目标名称” PUBLIC jhandler)
```

## 如何使用？

可以参考 `sample` 目录中的 `simpleUse`（常规使用） 和 `diyThread`（自定义线程） 方法。

## 常规使用

`JHandler` 中已经准备好了 `HandlerThread` ，**内部会在调用 `start` 方法后创建独立线程**，按照放入顺序串行分发事件消息或执行闭包。

```cpp
// 创建 HandlerThread
auto handlerThread = HandlerThread::create();
// 启动 HandlerThread
handlerThread->start();

// 对 HandlerThread 进行使用

// 退出 HandlerThread
handlerThread->quit();
```

**`quit` 方法会阻塞当前线程，直到 `HandlerThread` 线程退出结束。**

### 自定义线程

通过自行创建线程，在线程中创建 `Looper` 并启动，`Looper` 会对放入的事件消息和闭包进行按顺序执行。

**自定义线程中使用以下代码：**

```cpp
// 在自行创建的线程中，创建 Looper
mLooper = Looper::create();

// 启动 Looper 进入循环处理
mLooper->loop();
```

调用 `Looper->loop()` 方法之后，自定义线程会进入循环处理状态，当其他线程调用 `Looper->quit()` 方法才会终止循环处理。

**其他线程中使用以下代码进行停止轮询：**

```cpp
mLooper->quit();
```

### 添加事件消息、闭包

启动 `HandlerThread` 后，可以放入**闭包**或**事件消息 `Message`** 。

**闭包：** 调用 `Handler->post(std::function<void()> fun)` 方法放入闭包，具体下所示：

```cpp
auto name = "江澎涌";
firstHandler->post([name]() {
    Log::i("Demo-simpleUse", "你好，", name, "，这是闭包的调用。", "Looper 线程 id=", this_thread::get_id());
});
```

**事件消息 Message：** 调用 `Handler->sendMessage(std::unique_ptr<Message> message)` 方法放入事件消息。

```cpp
auto message = Message::obtain();                   // 获取事件消息
message->what = MSG_SAY_HELLO_WORLD;                // 事件类别
message->data = std::make_shared<string>("江澎涌");  // 事件数据
firstHandler->sendMessage(std::move(message));      // 放入事件消息
```

如何处理事件消息，请查看 **“自定义事件处理的 Handler”** 小节。

### 移除消息

可以通过 `Handler->removeMessage(int32_t what)` 移除与 `what` 相同的事件消息。

可以通过 `Handler->removeAllMessages()` 移除所有的闭包和事件消息。

### 自定义事件处理的 Handler

放入的事件消息，需要开发者自定义事件处理 Handler 进行接收处理。

在重写的 `handleMessage` 方法中接收放入的事件消息，然后根据事件消息执行相应业务的逻辑。

```cpp
// 获取 Looper 创建 Handler
auto looper = handlerThread->getLooper();
auto firstHandler = std::make_shared<MyFirstHandler>(looper);


class MyFirstHandler : public jhandler::Handler {
public:
    explicit MyFirstHandler(std::shared_ptr<jhandler::Looper> looper) : jhandler::Handler(std::move(looper)) {}

    void handleMessage(const std::unique_ptr<jhandler::Message> &message) override {
        // 处理事件，执行你需要处理的逻辑
        switch (message->what) {
            case MSG_SAY_HELLO_WORLD: {
                auto name = static_pointer_cast<string>(message->data);
                Log::i(TAG, *name, "说：\"世界，你好。\"", "Looper 线程 id=", this_thread::get_id());
                break;
            }
        }
    }
};

```