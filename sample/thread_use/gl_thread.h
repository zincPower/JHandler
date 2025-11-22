//
// Created by 江澎涌 on 2025/11/20.
//

#ifndef JHANDLER_SAMPLE_GL_THREAD_H
#define JHANDLER_SAMPLE_GL_THREAD_H

#include <looper.h>
#include <thread>
#include "gl_handler.h"

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
    std::shared_ptr<jhandler::Looper> mLooper = nullptr;
    std::shared_ptr<GLHandler> mHandler = nullptr;

public:
    bool isRunning();

    bool start();

    void quit();

    std::shared_ptr<jhandler::Looper> getLooper();

    void waitUntilReady();

private:
    static void loop(const std::shared_ptr<GLThread> &glThread) ;

    static void quitLoop(const std::shared_ptr<GLThread> &glThread);
};

#endif //JHANDLER_SAMPLE_GL_THREAD_H
