//
// Created by 江澎涌 on 2024/6/6.
//

#include "DiyThread.h"
#include "Looper.h"
#include "Handler.h"
#include "Log.h"
#include <thread>
#include <memory>
#include <string>

using namespace std;
using namespace jhandler;

class GLThread;

class GLHandler : public Handler {
private:
    static const std::string TAG;
    static constexpr int MSG_REQUEST_RENDER = 10000;

public:
    explicit GLHandler(std::shared_ptr<jhandler::Looper> looper) : jhandler::Handler(std::move(looper)) {}

    ~GLHandler() override = default;

    void handleMessage(const std::unique_ptr<jhandler::Message> &message) override {
        switch (message->what) {
            case MSG_REQUEST_RENDER:
                Log::i(TAG, "进行渲染 thread id=", this_thread::get_id());
                break;
        }
    }

    void sendRequestRender() {
        auto message = Message::obtain();
        message->what = MSG_REQUEST_RENDER;
        sendMessage(std::move(message));
    }
};

const std::string GLHandler::TAG = "GLHandler";

// 模拟 GL 的线程
class GLThread : public std::enable_shared_from_this<GLThread> {
private:
    static const std::string TAG;

    // 是否已经启动
    std::atomic<bool> mIsRunning = false;

    // 是否已经准备好环境
    std::atomic<bool> mIsThreadReady = false;
    std::mutex mReadyMutex{};
    std::condition_variable mReadyCond{};

    std::thread mThread;
    std::shared_ptr<Looper> mLooper = nullptr;
    std::shared_ptr<GLHandler> mHandler = nullptr;

public:
    bool isRunning() {
        return mIsRunning;
    }

    bool start() {
        if (mIsRunning) {
            Log::e(TAG, "GLThread is running. Can't start again.");
            return false;
        } else {
            Log::i(TAG, "GLThread start.");
            mIsRunning = true;
            mThread = std::thread(&loop, shared_from_this());
            return true;
        }
    }

    void quit() {
        Log::i(TAG, "Quit GLThread. mIsRunning=", mIsRunning ? "true" : "false");
        if (mIsRunning) mLooper->quit();
        // 避免 thread 游离导致退出时崩溃
        if (mThread.joinable()) mThread.detach();
    }

    std::shared_ptr<jhandler::Looper> getLooper() {
        return mLooper;
    }

    void requestRender() {
        if (mHandler) mHandler->sendRequestRender();
    }

    void waitUntilReady() {
        std::unique_lock<std::mutex> lock(mReadyMutex);
        mReadyCond.wait(lock, [self = shared_from_this()]() { return self->mIsThreadReady.load(); });
    }

private:
    static void loop(const std::shared_ptr<GLThread> &glThread) {
        Log::i(TAG, "------------------------ Enter GLThread. Start GL init logic. ------------------------");
        Log::i(TAG, "GLThread id=", std::this_thread::get_id());

        Log::i(TAG, "------------------------ 创建 EGL 相关环境 ------------------------");

        glThread->mLooper = Looper::create();
        glThread->mHandler = std::make_shared<GLHandler>(glThread->mLooper);

        // 用于等待 EGL、Looper、Handler 的创建完成
        {
            std::unique_lock<std::mutex> lock(glThread->mReadyMutex);
            glThread->mIsThreadReady = true;
            glThread->mReadyCond.notify_all();
        }

        Log::i(TAG, "------------------------ 进入事件循环 ------------------------");
        glThread->mLooper->loop();
        Log::i(TAG, "------------------------ 退出事件循环 ------------------------");

        Log::i(TAG, "------------------------ 开始释放资源 ------------------------");


        Log::i(TAG, "------------------------ 释放 EGL ------------------------ ");

        Log::i(TAG, "------------------------ 释放 Handler ------------------------ ");
        glThread->mHandler->removeAllMessages();
        glThread->mHandler = nullptr;

        Log::i(TAG, "释放 Looper");
        glThread->mLooper = nullptr;
        quitLoop(glThread);
        Log::i(TAG, "------------------------ 退出 GLThread 线程 ------------------------");
    }

    static void quitLoop(const std::shared_ptr<GLThread> &glThread) {
        std::unique_lock<std::mutex> lock(glThread->mReadyMutex);
        Log::i(TAG, "Loop quit.");
        glThread->mIsRunning = false;
        glThread->mIsThreadReady = false;
        glThread->mReadyCond.notify_all();
    }
};

const std::string GLThread::TAG = "GLThread";

class MyHandler : public Handler {
private:
    static const std::string TAG;
    static constexpr int MSG_ADD_FILTER = 10000;
    static constexpr int MSG_REMOVE_FILTER = 10001;

public:
    explicit MyHandler(std::shared_ptr<jhandler::Looper> looper) : jhandler::Handler(std::move(looper)) {}

    ~MyHandler() override = default;

    void handleMessage(const std::unique_ptr<jhandler::Message> &message) override {
        switch (message->what) {
            case MSG_ADD_FILTER:
                Log::i(TAG, "模拟添加滤镜 thread id=", this_thread::get_id(), ", filter name=", message->data);
                break;
            case MSG_REMOVE_FILTER:
                Log::i(TAG, "模拟移除滤镜 thread id=", this_thread::get_id(), ", filter name=", message->data);
                break;
        }
    }

    void addFilter() {
        auto message = Message::obtain();
        message->what = MSG_ADD_FILTER;
        message->data = std::make_shared<std::string>("TriangleFilter");
        sendMessage(std::move(message));
    }

    void removeFilter() {
        auto message = Message::obtain();
        message->what = MSG_REMOVE_FILTER;
        message->data = std::make_shared<std::string>("TriangleFilter");
        sendMessage(std::move(message));
    }
};

const std::string MyHandler::TAG = "MyHandler";

void diyThread() {
    Log::i("Demo-diyThread", "=======================================================================================");
    Log::i("Demo-diyThread", "==================================== 自定义线程 开始 ====================================");
    Log::i("Demo-diyThread", "=======================================================================================");

    auto glThread = std::make_shared<GLThread>();
    if (glThread->start()) {
        glThread->waitUntilReady();
    }
    glThread->requestRender();

    auto myHandler = std::make_shared<MyHandler>(glThread->getLooper());
    myHandler->addFilter();
    myHandler->removeFilter();

    this_thread::sleep_for(std::chrono::microseconds(50));

    glThread->quit();

    Log::i("Demo-diyThread", "=======================================================================================");
    Log::i("Demo-diyThread", "==================================== 自定义线程 结束 ====================================");
    Log::i("Demo-diyThread", "=======================================================================================");
}