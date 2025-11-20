//
// Created by 江澎涌 on 2024/6/2.
//

#ifndef JHANDLER_CPP_MESSAGE_H
#define JHANDLER_CPP_MESSAGE_H

#include <memory>
#include <functional>
#include <iostream>

namespace jhandler {
class Handler;

class Message;

class Message {
public:
    friend std::ostream &operator<<(std::ostream &os, const Message &msg);

    friend class Looper;

    friend class Message;

    friend class Handler;

    friend class MessageQueue;

public:
    std::int32_t what = 0;
    std::int32_t arg1 = 0;
    std::int32_t arg2 = 0;
    std::shared_ptr<void> data = nullptr;

private:
    static const std::string TAG;

    std::shared_ptr<Handler> mTarget = std::shared_ptr<Handler>();
    std::function<void()> mCallback = nullptr;
    
public:
    static std::unique_ptr<Message> obtain();

    void recycle();

    Message(const Message &) = delete;

    Message &operator=(const Message &) = delete;

    Message(Message &&) = delete;

    Message &operator=(Message &&) = delete;

    ~Message();

    template<typename T>
    T *getData() const {
        if (!data) return nullptr;
        return static_cast<T *>(data.get());
    }

private:
    Message();

    void reset();
};
}

#endif //JHANDLER_CPP_MESSAGE_H
