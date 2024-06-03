//
// Created by 江澎涌 on 2024/6/2
//

#ifndef JHANDLER_CPP_HANDLERTHREAD_H
#define JHANDLER_CPP_HANDLERTHREAD_H

#include <memory>
#include <thread>
#include "Looper.h"

namespace jhandler {
class HandlerThread {
private:
    static const std::string TAG;

    std::thread mThread{};
    std::shared_ptr<Looper> mLooper = nullptr;
public:
    HandlerThread();

    ~HandlerThread();

    void start();

    void quit();

    std::shared_ptr<Looper> getLooper();

    void join();

    void detach();

private:
    static void loop(std::shared_ptr<Looper> looper);
};
}

#endif //JHANDLER_CPP_HANDLERTHREAD_H
