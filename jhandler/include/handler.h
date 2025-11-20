//
// Created by 江澎涌 on 2024/6/2.
//

#ifndef JHANDLER_CPP_HANDLER_H
#define JHANDLER_CPP_HANDLER_H

#include <memory>
#include <functional>
#include "message.h"
#include "looper.h"

namespace jhandler {
class Handler : public std::enable_shared_from_this<Handler> {
private:
    static const std::string TAG;

    std::shared_ptr<Looper> mLooper = nullptr;
public:
    static std::unique_ptr<Message> obtainMessage();

    explicit Handler(std::shared_ptr<Looper> looper);

    virtual ~Handler();

    void post(std::function<void()> fun);

    void sendMessage(std::unique_ptr<Message> message);

    void removeMessage(int32_t what);

    void removeAllMessages();

    virtual void handleMessage(const std::unique_ptr<Message> &message) = 0;
};
}

#endif //JHANDLER_CPP_HANDLER_H
