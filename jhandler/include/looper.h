//
// Created by 江澎涌 on 2024/6/2.
//

#ifndef JHANDLER_CPP_LOOPER_H
#define JHANDLER_CPP_LOOPER_H

#include <memory>
#include <atomic>
#include "../src/message_queue.h"

namespace jhandler {
class Looper {
private:
    static const std::string TAG;

    std::unique_ptr<MessageQueue> mQueue;
public:
    static std::shared_ptr<Looper> create();

    void loop();

    void quit();

    void enqueueMessage(std::unique_ptr<Message> message);

    void removeMessage(std::shared_ptr<Handler> handler, int32_t what);

    void removeAllMessages(std::shared_ptr<Handler> handler);

    Looper(const Looper &) = delete;

    Looper &operator=(const Looper &) = delete;

    ~Looper() = default;

private:
    Looper();
};
}

#endif //JHANDLER_CPP_LOOPER_H
