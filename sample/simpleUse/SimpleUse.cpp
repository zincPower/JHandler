//
// Created by 江澎涌 on 2024/6/5.
//

#include "SimpleUse.h"

#include <string>
#include <utility>
#include "Log.h"
#include "HandlerThread.h"

using namespace std;
using namespace jhandler;

static constexpr int MSG_SAY_HELLO_WORLD = 10000;
static constexpr int MSG_SAY_BEY = 20000;

class MySimpleUseHandler : public jhandler::Handler {
private:
    string TAG = "Demo-MySimpleUseHandler";
public:
    explicit MySimpleUseHandler(std::shared_ptr<jhandler::Looper> looper) : jhandler::Handler(std::move(looper)) {}

    void handleMessage(const std::unique_ptr<jhandler::Message> &message) override {
        switch (message->what) {
            case MSG_SAY_HELLO_WORLD:
                Log::i(TAG, "Hello, world.", "Looper 线程 id=", this_thread::get_id());
                break;
            case MSG_SAY_BEY:
                Log::i(TAG, "Bey.", "Looper 线程 id=", this_thread::get_id());
                break;
        }
    }
};

/**
 * 简单使用例子：
 * 使用 HandlerThread ，通过
 */
void simpleUse() {
    auto handlerThread = HandlerThread::create();
    handlerThread->start();

    auto looper = handlerThread->getLooper();
    auto handler = std::make_shared<MySimpleUseHandler>(looper);

    // 传递不同的 what Message
    auto message = Message::obtain();
    message->what = MSG_SAY_HELLO_WORLD;
    handler->sendMessage(std::move(message));

    message = Message::obtain();
    message->what = MSG_SAY_BEY;
    handler->sendMessage(std::move(message));

    // 闭包携带外部参数
    auto name = "江澎涌";
    handler->post([name]() {
        Log::i("Demo-simpleUse", "你好，", name, "，这是闭包的调用。", "Looper 线程 id=", this_thread::get_id());
    });

    // HandlerThread 进行 quit ，采取线程 detach ，所以并不会让当前线程等待 HandlerThread 内部线程退出完成在执行后续代码
    // 所以此处进行睡眠 2 秒，否则整个程序快速退出，会导致没有看到内部执行
    this_thread::sleep_for(std::chrono::seconds(2));

    handlerThread->quit();
}