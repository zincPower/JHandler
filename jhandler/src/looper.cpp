//
// Created by 江澎涌 on 2024/6/2.
//

#include "looper.h"
#include "handler.h"
#include "log.h"

namespace jhandler {
const std::string Looper::TAG = "Looper";

std::shared_ptr<Looper> Looper::create() {
    return std::shared_ptr<Looper>(new Looper());
}

void Looper::loop() {
    Log::i(TAG, "Start loop.");

    while (true) {
        // 这里会将所有的任务都处理完成了才退出
        auto message = mQueue->next();
        if (message) {
            Log::i(TAG, "Obtain a message.", *message);
            if (message->mCallback) {
                message->mCallback();
            } else {
                auto target = message->mTarget;
                if (target) target->handleMessage(message);
            }
        } else if (mQueue->isQuit()) {
            Log::i(TAG, "Looper is stopped.");
            break;
        } else {
            Log::e(TAG, "Looper get a nullptr message.");
        }
    }

    // 退出后，重置队列状态，可以让队列继续接收数据
    mQueue->reset();

    Log::i(TAG, "Finish loop.");
}

void Looper::quit() {
    mQueue->quit();
}

void Looper::enqueueMessage(std::unique_ptr<Message> message) {
//    Log::i(TAG, "Enqueue a message.", *message);
    mQueue->enqueueMessage(std::move(message));
}

void Looper::removeMessage(std::shared_ptr<Handler> handler, int32_t what) {
    mQueue->removeMessage(handler, what);
}

void Looper::removeAllMessages(std::shared_ptr<Handler> handler) {
    mQueue->removeAllMessages(handler);
}

Looper::Looper() {
    Log::i(TAG, "Create looper.");
    mQueue = std::make_unique<MessageQueue>();
}
}