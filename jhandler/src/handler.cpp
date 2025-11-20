//
// Created by 江澎涌 on 2024/6/2.
//

#include "handler.h"
#include "log.h"

namespace jhandler{
const std::string Handler::TAG = "Handler";

std::unique_ptr<Message> Handler::obtainMessage() {
    return Message::obtain();
}

Handler::Handler(std::shared_ptr<Looper> looper) : mLooper(std::move(looper)) {
    Log::i(TAG, "Create handler.");
}

Handler::~Handler() {
    Log::i(TAG, "Delete handler.");
    mLooper = nullptr;
}

void Handler::post(std::function<void()> fun) {
    auto message = obtainMessage();
    message->mTarget = shared_from_this();
    message->mCallback = std::move(fun);
    mLooper->enqueueMessage(std::move(message));
}

void Handler::sendMessage(std::unique_ptr<Message> message) {
    message->mTarget = shared_from_this();
    mLooper->enqueueMessage(std::move(message));
}

void Handler::removeMessage(int32_t what) {
    mLooper->removeMessage(shared_from_this(), what);
}

void Handler::removeAllMessages() {
    mLooper->removeAllMessages(shared_from_this());
}
} // namespace handler