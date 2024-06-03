//
// Created by 江澎涌 on 2024/6/2.
//

#include "Looper.h"
#include "Handler.h"
#include "Log.h"

namespace jhandler{
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
                // 【Message 回收】 可以考虑将 message 回收
            } else {
                auto target = message->mTarget;
                auto locked = target.lock();
                if (locked) locked->handleMessage(message);
                // 【Message 回收】 可以考虑将 message 回收
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
    Log::i(TAG, "Enqueue a message.", *message);
    mQueue->enqueueMessage(std::move(message));
}

void Looper::removeMessage(int32_t what) {
    mQueue->removeMessage(what);
}

void Looper::removeAllMessages() {
    mQueue->removeAllMessages();
}

Looper::Looper() {
    Log::i(TAG, "Create looper.");
    mQueue = std::make_unique<MessageQueue>();
}
}