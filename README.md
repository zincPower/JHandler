# 零、JHandler

**JHandler 是一套 C++ 的事件循环机制。** 可在自行创建的线程中使用，也可以使用 JHandler 已封装好的独立线程处理事件。

## 一、集成

将项目中 `jhandler` 文件夹拷贝添加到项目中，并在项目的 `CMakeLists.txt` 中添加以下配置即可：

```cmake
include_directories(“添加的 jhandler 目录相对于当前 CMakeLists.txt 文件的路径”/jhandler/include)
add_subdirectory(jhandler)

target_link_libraries(“链接目标名称” PUBLIC jhandler)
```

## 二、常规使用

### 1、使用 JHandler 自带的线程

`JHandler` 中已经准备好了 `HandlerThread` ，**内部会在调用 `start` 方法后创建独立线程**，按照放入消息和闭包的顺序，串行分发事件消息或执行闭包。

```cpp
// 创建 HandlerThread
auto handlerThread = jhandler::HandlerThread::create();
// 启动 HandlerThread ，内部会启动线程
handlerThread->start();

// 对 HandlerThread 进行使用

// 退出 HandlerThread ，会执行完已经放入的事件消息和闭包
handlerThread->quit();
```

**`quit` 方法不会阻塞当前线程，会让 `HandlerThread` 内部线程在处理完所有的消息和闭包后关闭线程。**

### 2、添加事件消息、闭包

启动 `HandlerThread` 后，可以放入**闭包**或**事件消息 `Message`** 。

**闭包：** 调用 `Handler->post(std::function<void()> fun)` 方法放入闭包，具体下所示：

```cpp
auto name = "江澎涌";
handler->post([name]() {
jhandler::Log::i(TAG, "【runClosure】运行闭包 name=", name, " Looper 线程 id=", std::this_thread::get_id());
});

// 输出
// 【CommonUse】 【runClosure】运行闭包 name=江澎涌 Looper 线程 id=0x700000339000
```

**事件消息 Message：** 调用 `Handler->sendMessage(std::unique_ptr<Message> message)` 方法放入事件消息。

```cpp
auto message = jhandler::Message::obtain();             // 获取事件消息
message->what = SAY_HI;                                 // 事件类别
message->data = std::make_shared<std::string>("江澎涌"); // 事件数据
message->arg1 = 1994;
message->arg2 = 170;
handler->sendMessage(std::move(message));               // 放入事件消息

// 输出
// 【FirstCommonUseHandler】 【handleMessage】你好，江澎涌(1994,170) Looper 线程 id=0x70000589c000
```

> 如何处理事件消息，请查看 **“自定义事件处理的 Handler”** 小节。

### 3、移除消息

可以通过 `Handler->removeMessage(int32_t what)` 移除与 `what` 相同的事件消息。

```cpp
handler->removeMessage(SAY_HI);
```

可以通过 `Handler->removeAllMessages()` 移除所有的闭包和事件消息。

```cpp
handler->removeAllMessages();
```

### 4、定义处理事件消息的 Handler

放入的事件消息，需要开发者继承 `jhandler::Handler` 编写自定义事件处理的 `Handler` 进行接收处理。

在重写的 `handleMessage` 方法中接收放入的事件消息，编写相应业务的逻辑。具体编写如下：

```cpp
// 定义事件消息 what
static constexpr int SAY_HI = 10000;

// 定义 Handler
class FirstCommonUseHandler : public jhandler::Handler {
private:
    static std::string TAG;
public:
    explicit FirstCommonUseHandler(std::shared_ptr<jhandler::Looper> looper) {}
    void handleMessage(const std::unique_ptr<jhandler::Message> &message) override{
        // 此处接收 Message 编写自己的业务逻辑
        switch (message->what) {
            case SAY_HI: {
                auto name = message->getData<std::string>();
                auto year = message->arg1;
                auto height = message->arg2;
                Log::i(TAG, "【handleMessage】你好，", *name, "(", year, ",", height, ")", " Looper 线程 id=", std::this_thread::get_id());
                break;
            }
            case SHOW_DESCRIPTION: {
                Log::i(TAG, "【handleMessage】我是一个 C++ 事件循环机制 Looper 线程 id=", std::this_thread::get_id());
                break;
            }
        }
    }
};

// 创建 HandlerThread
auto handlerThread = jhandler::HandlerThread::create();
// 启动 HandlerThread
handlerThread->start();
// 获取 Looper
auto looper = handlerThread->getLooper();
// 创建自己的 Handler
auto handler = std::make_shared<FirstCommonUseHandler>(looper);
// 事件消息传递
auto message = jhandler::Message::obtain();
message->what = SAY_HI;
message->data = std::make_shared<std::string>("江澎涌");
message->arg1 = 1994;
message->arg2 = 170;
handler->sendMessage(std::move(message));

message = jhandler::Message::obtain();
message->what = SHOW_DESCRIPTION;
handler->sendMessage(std::move(message));

// 输出 
// 【FirstCommonUseHandler】 【handleMessage】你好，江澎涌(1994,170) Looper 线程 id=0x70000589c000
// 【FirstCommonUseHandler】 【handleMessage】我是一个 C++ 事件循环机制 Looper 线程 id=0x70000589c000
```

### 5、多个 Handler 解耦逻辑

有时需要将处理事件消息的逻辑解耦，可以考虑通过 `Looper` 创建多个 `Handler` ，将事件消息发送到对应的 `Handler` ，后续会由该 `Handler` 执行对应逻辑，并且**多个 `Handler` 都是运行在同一个线程中，按添加顺序进行执行。**

具体编写如下：

```cpp
auto handlerThread = jhandler::HandlerThread::create();
handlerThread->start();

auto looper = handlerThread->getLooper();
// 创建两个 Handler
auto handler1 = std::make_shared<FirstCommonUseHandler>(looper);
auto handler2 = std::make_shared<SecondCommonUseHandler>(looper);

// 向 handler1 发送 SAY_HI 类型的 message ，由 handler1 进行处理 
auto message = jhandler::Message::obtain();
message->what = SAY_HI;
message->data = std::make_shared<std::string>("江澎涌");
message->arg1 = 1994;
message->arg2 = 170;
handler1->sendMessage(std::move(message));

// 向 handler2 发送 SAY_HI 类型的 message ，由 handler2 进行处理 
message = jhandler::Message::obtain();
message->what = SAY_HI;
message->data = std::make_shared<std::string>("jiang peng yong");
message->arg1 = 2025;
message->arg2 = 100;
handler2->sendMessage(std::move(message));

// 会看到以下输出，虽然是同一类型消息，但由不同 Handler 进行处理，并且线程是相同的且按顺序执行
//【FirstCommonUseHandler】 【handleMessage】你好，江澎涌(1994,170) Looper 线程 id=0x700009ae3000
//【SecondCommonUseHandler】 【handleMessage】hello, jiang peng yong(2025,100) Looper 线程 id=0x700009ae3000
```

> `FirstCommonUseHandler` 和 `SecondCommonUseHandler` 请查看源码不再赘述。

## 三、自定义线程中使用 JHandler

有些情况下，需要在自己的线程中使用事件循环机制，所以 JHandler 也支持在自定义线程中增加事件循环机制。

例如在鸿蒙中，需要封装一个 OpenGL 线程给到相机使用，OpenGL 是线程相关的，需要一个事件循环不断的处理每一帧数据，同时处理好 Surface 、滤镜等数据。以下便是一个 OpenGL 的模拟样例：

> 完整代码可以查看 `thread_use.cpp`

**在自定义线程中，按照如下步骤进行：**

1. 增加 egl 的创建，通过 `jhandler::Looper::create()` 创建 `Looper` ，然后创建内部的 Handler 用于处理后续的相机帧、滤镜管理等。
2. 调用 `Looper::loop()` 方法，进入事件循环，直到外部调用 `Looper::quit()` 终止事件循环。
3. 释放和回收 egl 相关资源。

```cpp
void GLThread::loop(const std::shared_ptr<GLThread> &glThread) {
    Log::i(TAG, "------------------------ 进入 GLThread 启动 GL 逻辑 ------------------------ thread id=", std::this_thread::get_id());

    Log::i(TAG, "------------------------ 模拟创建 EGL 相关环境 ------------------------ thread id=", std::this_thread::get_id());
    // 睡眠了 500 毫秒，模拟创建 EGL
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    Log::i(TAG, "------------------------ 进入事件循环 ------------------------ thread id=", std::this_thread::get_id());
    glThread->mLooper->loop();
    Log::i(TAG, "------------------------ 退出事件循环 ------------------------ thread id=", std::this_thread::get_id());

    Log::i(TAG, "------------------------ 开始释放资源 ------------------------ thread id=", std::this_thread::get_id());

    Log::i(TAG, "------------------------ 释放 EGL ------------------------ thread id=", std::this_thread::get_id());

    Log::i(TAG, "------------------------ 释放 Handler ------------------------ thread id=", std::this_thread::get_id());
    glThread->mHandler->removeAllMessages();
    glThread->mHandler = nullptr;

    Log::i(TAG, "释放 Looper");
    glThread->mLooper = nullptr;
    quitLoop(glThread);
    Log::i(TAG, "------------------------ 退出 GLThread 线程 ------------------------ thread id=", std::this_thread::get_id());
}
```

同样也支持多个 Handler 解耦逻辑，通过获取内部的 `Looper` 创建对应的 `Handler` 即可。

```cpp
void threadUse() {
    auto glThread = std::make_shared<GLThread>();
    glThread->start();

    // 创建业务需要的 handler ，可以不耦合 gl 的相关流程
    auto businessHandler = std::make_shared<BusinessHandler>(glThread->getLooper());
    businessHandler->sayHello();

    auto glHandler = glThread->getHandler();
    glHandler->addFilter();
    glHandler->requestRender();
    glHandler->removeFilter();

    businessHandler->sayHello();

    glThread->quit();

    // 为了让内部执行外，才结束整个项目运行。
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // 输出
    // 【GLHandler】 添加滤镜 filterName=0x600002314048 thread id=0x700009ae3000
    // 【GLHandler】 进行渲染 thread id=0x700009ae3000
    // 【GLHandler】 移除滤镜 filterName=0x600002314078 thread id=0x700009ae3000
    // 【BusinessHandler】 你好 thread id=0x700009ae3000
}
```

## 六、作者简介

### 1、个人博客

掘金：https://juejin.im/user/5c3033ef51882524ec3a88ba/posts

csdn：https://blog.csdn.net/weixin_37625173

公众号：微信搜索 "江澎涌" ，或扫描二维码

![](https://github.com/zincPower/JHandler/blob/main/img/officialaccount.png)

### 2、赞赏

如果觉得 JHandler 对你有帮助或启发，请我喝杯水果茶吧 😄

![](https://github.com/zincPower/JHandler/blob/main/img/pay.jpg)
