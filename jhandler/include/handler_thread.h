//
// Created by 江澎涌 on 2024/6/2
//

#ifndef JHANDLER_CPP_HANDLERTHREAD_H
#define JHANDLER_CPP_HANDLERTHREAD_H

#include <memory>
#include <thread>
#include "looper.h"

namespace jhandler {
class HandlerThread {
private:
    static const std::string TAG;

    std::thread mThread{};
    std::shared_ptr<Looper> mLooper = nullptr;
public:
    static std::shared_ptr<HandlerThread> create();

    ~HandlerThread();

    void start();

    void quit();

    std::shared_ptr<Looper> getLooper();

private:
    HandlerThread();

    static void loop(const std::shared_ptr<Looper>& looper);
};
}

#endif //JHANDLER_CPP_HANDLERTHREAD_H
