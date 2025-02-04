//
// Created by 江澎涌 on 2024/6/5.
//

#include "SimpleUse.h"

#include <memory>
#include <string>
#include <utility>
#include "Log.h"
#include "HandlerThread.h"

using namespace std;
using namespace jhandler;

static constexpr int MSG_SAY_HELLO_WORLD = 10000;
static constexpr int MSG_SAY_BEY = 20000;
static constexpr int MSG_WILL_BE_REMOVED = 30000;

class MyFirstHandler : public jhandler::Handler {
private:
    string TAG = "Demo-MyFirstHandler";
public:
    explicit MyFirstHandler(std::shared_ptr<jhandler::Looper> looper) : jhandler::Handler(std::move(looper)) {}

    void handleMessage(const std::unique_ptr<jhandler::Message> &message) override {
        switch (message->what) {
            case MSG_SAY_HELLO_WORLD: {
                auto name = static_pointer_cast<string>(message->data);
                Log::i(TAG, *name, " say: \"Hello, world.\"", "Looper 线程 id=", this_thread::get_id());
                break;
            }
            case MSG_SAY_BEY: {
                Log::i(TAG, "Say Bey.", "Looper 线程 id=", this_thread::get_id());
                break;
            }
            case MSG_WILL_BE_REMOVED: {
                Log::i(TAG, "Will be removed message.", "Looper 线程 id=", this_thread::get_id());
                break;
            }
        }
    }
};

class MySecondHandler : public jhandler::Handler {
private:
    string TAG = "Demo-MySecondHandler";
public:
    explicit MySecondHandler(std::shared_ptr<jhandler::Looper> looper) : jhandler::Handler(std::move(looper)) {}

    void handleMessage(const std::unique_ptr<jhandler::Message> &message) override {
        switch (message->what) {
            case MSG_SAY_HELLO_WORLD: {
                auto name = static_pointer_cast<string>(message->data);
                Log::i(TAG, *name, "说：\"世界，你好。\"", "Looper 线程 id=", this_thread::get_id());
                break;
            }
            case MSG_SAY_BEY: {
                Log::i(TAG, "说再见。", "Looper 线程 id=", this_thread::get_id());
                break;
            }
            case MSG_WILL_BE_REMOVED: {
                Log::i(TAG, "将会被移除的信息", "Looper 线程 id=", this_thread::get_id());
                break;
            }
        }
    }
};

/**
 * 简单使用例子：
 * 使用 HandlerThread ，通过
 */
void simpleUse() {
    Log::i("Demo-simpleUse", "=====================================================================================");
    Log::i("Demo-simpleUse", "================================== 开箱即用例子 开始 ==================================");
    Log::i("Demo-simpleUse", "=====================================================================================");

    auto handlerThread = HandlerThread::create();
    handlerThread->start();

    auto looper = handlerThread->getLooper();
    auto firstHandler = std::make_shared<MyFirstHandler>(looper);

    // 传递不同的 what Message
    auto message = Message::obtain();
    message->what = MSG_SAY_HELLO_WORLD;
    message->data = std::make_shared<string>("江澎涌");
    firstHandler->sendMessage(std::move(message));

    message = Message::obtain();
    message->what = MSG_SAY_BEY;
    firstHandler->sendMessage(std::move(message));

    // 闭包携带外部参数
    auto name = "江澎涌";
    firstHandler->post([name]() {
        Log::i("Demo-simpleUse", "你好，", name, "，这是闭包的调用。", "Looper 线程 id=", this_thread::get_id());
    });

    // 移除对应的 Message ，MSG_WILL_BE_REMOVED 的 Message 来不及运行，则可以进行实际性移除，会看到
    // 【MessageQueue】 MessageQueue has been removed message. message= [Message what=30000, arg1=0, arg2=0, data is nullptr, callback is nullptr.]
    // 如果执行了，则无法移除该 Message
    message = Message::obtain();
    message->what = MSG_WILL_BE_REMOVED;
    firstHandler->sendMessage(std::move(message));
    firstHandler->removeMessage(MSG_WILL_BE_REMOVED);

    // 移除所有的 Message ，同样来不及运行的 Message 则会在全部被移除，则会看到以下提示：
    // 【MessageQueue】 MessageQueue remove all messages. Queue size=1
    // 这个因设备性能决定，所以在 removeAllMessages 前进行了睡眠，否则会立马把所有的信息都移除
    message = Message::obtain();
    message->what = MSG_SAY_HELLO_WORLD;
    message->data = std::make_shared<string>("jiang peng yong");
    firstHandler->sendMessage(std::move(message));
    message = Message::obtain();
    message->what = MSG_WILL_BE_REMOVED;
    firstHandler->sendMessage(std::move(message));
    this_thread::sleep_for(std::chrono::microseconds(50));
    firstHandler->post([]() {
        Log::i("Demo-simpleUse", "被移除的闭包", "Looper 线程 id=", this_thread::get_id());
    });
    firstHandler->removeAllMessages();

    // 用同一个 Looper 创建多个 Handler ，可以分散处理逻辑
    // 并且移除消息和发送消息互不影响
    auto secondHandler = std::make_shared<MySecondHandler>(looper);
    message = Message::obtain();
    message->what = MSG_SAY_HELLO_WORLD;
    message->data = std::make_shared<string>("江澎涌");
    secondHandler->sendMessage(std::move(message));
    message = Message::obtain();
    message->what = MSG_SAY_BEY;
    secondHandler->sendMessage(std::move(message));
    firstHandler->removeAllMessages();

    // HandlerThread 进行 quit ，采取线程 detach ，所以并不会让当前线程等待 HandlerThread 内部线程退出完成在执行后续代码
    // 所以此处进行睡眠 2 秒，否则整个程序快速退出，会导致没有看到内部执行
    this_thread::sleep_for(std::chrono::seconds(2));

    handlerThread->quit();
    Log::i("Demo-simpleUse", "=====================================================================================");
    Log::i("Demo-simpleUse", "================================== 开箱即用例子 结束 ==================================");
    Log::i("Demo-simpleUse", "=====================================================================================");
}