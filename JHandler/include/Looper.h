//
// Created by 江澎涌 on 2024/6/2.
//

#ifndef JHANDLER_CPP_LOOPER_H
#define JHANDLER_CPP_LOOPER_H

#include <memory>
#include <atomic>
#include "MessageQueue.h"

namespace jhandler {
class Looper {
private:
    static const std::string TAG;
    
    std::unique_ptr<MessageQueue> mQueue;
public:
    static std::shared_ptr<Looper> create();

    // android 中，是为了让 Looper 和当前线程绑定，这里暂不考虑，所以可以先不用
    // void prepare();

    void loop();

    void quit();

    void enqueueMessage(std::unique_ptr<Message> message);

    void removeMessage(int32_t what);

    void removeAllMessages();

    Looper(const Looper &) = delete;

    Looper &operator=(const Looper &) = delete;

    ~Looper() = default;

private:
    Looper();
};
}

#endif //JHANDLER_CPP_LOOPER_H
