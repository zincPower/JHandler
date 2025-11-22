# JHandler

JHandler 是一套 C++ 的事件循环机制。可在自行创建的线程中使用，也可以使用 JHandler 已封装好的独立线程处理事件。

## 集成

将 `jhandler` 文件夹拷贝添加到项目中，并在项目的 `CMakeLists.txt` 中添加以下配置即可：

```cmake
include_directories(“添加的 jhandler 目录相对于当前 CMakeLists.txt 文件的路径”/jhandler/include)
add_subdirectory(jhandler)

target_link_libraries(“链接目标名称” PUBLIC jhandler)
```

## 如何启动 JHandler

### 1、使用 JHandler 自带的线程

`JHandler` 中已经准备好了 `HandlerThread` ，**内部会在调用 `start` 方法后创建独立线程**，按照放入消息和闭包的顺序，串行分发事件消息或执行闭包。

```cpp
// 创建 HandlerThread
auto handlerThread = jhandler::HandlerThread::create();
// 启动 HandlerThread ，内部会启动线程
handlerThread->start();

// 对 HandlerThread 进行使用

// 退出 HandlerThread ，会执行完
handlerThread->quit();
```

**`quit` 方法不会阻塞当前线程，会让 `HandlerThread` 内部线程在处理完所有的消息和闭包后关闭线程。**

### 2、自定义线程中使用 JHandler

在需要使用事件循环的线程中使用 `Looper::create` 创建 `Looper` 并调用 `loop` 启动，`Looper` 会对放入的消息和闭包按照顺序执行。

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

## 事件消息和闭包的使用

### 1、添加事件消息、闭包

启动 `HandlerThread` 后，可以放入**闭包**或**事件消息 `Message`** 。

**闭包：** 调用 `Handler->post(std::function<void()> fun)` 方法放入闭包，具体下所示：

```cpp
auto name = "江澎涌";
handler->post([name]() {
jhandler::Log::i(TAG, "【runClosure】运行闭包 name=", name, " Looper 线程 id=", std::this_thread::get_id());
});
```

**事件消息 Message：** 调用 `Handler->sendMessage(std::unique_ptr<Message> message)` 方法放入事件消息。

```cpp
auto message = jhandler::Message::obtain();             // 获取事件消息
message->what = SAY_HI;                                 // 事件类别
message->data = std::make_shared<std::string>("江澎涌"); // 事件数据
handler->sendMessage(std::move(message));               // 放入事件消息
```

如何处理事件消息，请查看 **“自定义事件处理的 Handler”** 小节。

### 2、移除消息

可以通过 `Handler->removeMessage(int32_t what)` 移除与 `what` 相同的事件消息。

```cpp
handler->removeMessage(SAY_HI);
```

可以通过 `Handler->removeAllMessages()` 移除所有的闭包和事件消息。

```cpp
handler->removeAllMessages();
```

## 定义处理事件消息的 Handler

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
                auto name = std::static_pointer_cast<std::string>(message->data);
                jhandler::Log::i(TAG, "【addMessage】处理事件消息 name=", *name, " Looper 线程 id=", std::this_thread::get_id());
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
handler->sendMessage(std::move(message));
```

**多个 Handler 解耦逻辑**

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
handler1->sendMessage(std::move(message));

// 向 handler2 发送 SAY_HI 类型的 message ，由 handler2 进行处理 
message = jhandler::Message::obtain();
message->what = SAY_HI;
message->data = std::make_shared<std::string>("jiang peng yong");
handler2->sendMessage(std::move(message));

// 会看到以下输出，虽然是同一类型消息，但由不同 Handler 进行处理，并且线程是相同的且按顺序执行
//【FirstCommonUseHandler】 【addMessage】处理事件消息 name=江澎涌 Looper 线程 id=0x700007d2d000
//【SecondCommonUseHandler】 【addMessage】处理事件消息 name=jiang peng yong Looper 线程 id=0x700007d2d000
```

> `FirstCommonUseHandler` 和 `SecondCommonUseHandler` 请查看源码不再赘述。

##