//
// Created by 江澎涌 on 2024/6/2.
//

#ifndef JHANDLER_CPP_MESSAGEQUEUE_H
#define JHANDLER_CPP_MESSAGEQUEUE_H

#include <mutex>
#include <condition_variable>
#include <list>
#include "Message.h"

namespace jhandler {
class MessageQueue {
private:
    static const std::string TAG;

    std::list<std::unique_ptr<Message>> mQueue = {};
    std::atomic<bool> mIsQuit = false;

    std::mutex mMutex;
    std::condition_variable mThreadCond;
public:
    MessageQueue();

    ~MessageQueue();

    void enqueueMessage(std::unique_ptr<Message> message);

    std::unique_ptr<Message> next();

    void removeMessage(int32_t what);

    void removeAllMessages();

    void reset();
    
    void quit();
    
    [[nodiscard]] bool isQuit();
};
}

#endif //JHANDLER_CPP_MESSAGEQUEUE_H
