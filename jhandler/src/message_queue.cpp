//
// Created by 江澎涌 on 2024/6/2.
//

#include "message_queue.h"
#include "log.h"

namespace jhandler {
const std::string MessageQueue::TAG = "MessageQueue";

MessageQueue::MessageQueue() {
    Log::i(TAG, "Create message queue.");
}

MessageQueue::~MessageQueue() {
    Log::i(TAG, "Delete message queue.");
}

void MessageQueue::enqueueMessage(std::unique_ptr<Message> message) {
    std::unique_lock<std::mutex> lock(mMutex);
    // 如果已经退出了，则无法再添加任务
    if (mIsQuit) {
        Log::i(TAG, "MessageQueue has been quit.");
        return;
    }
    Log::i(TAG, "MessageQueue add a message. message=", *message);
    mQueue.push_back(std::move(message));
    mThreadCond.notify_all();
}

std::unique_ptr<Message> MessageQueue::next() {
    std::unique_lock<std::mutex> lock(mMutex);
    // 如果队列不为空或已经退出，则不在进行等待
    mThreadCond.wait(lock, [this]() { return !mQueue.empty() || mIsQuit; });
    // 队列不为空，则执行完所有的任务之后在退出，所以先检查队列是否为空
    if (!mQueue.empty()) {
        std::unique_ptr<Message> message = std::move(mQueue.front());
        mQueue.pop_front();
        Log::i(TAG, "MessageQueue get a new message. message=", *message);
        return message;
    } else {
        Log::i(TAG, "MessageQueue is stopped. isQuited=", mIsQuit ? "true" : "false");
        return nullptr;
    }
}

void MessageQueue::removeMessage(std::shared_ptr<Handler> handler, int32_t what) {
    std::unique_lock<std::mutex> lock(mMutex);
    Log::i(TAG, "MessageQueue remove message. what=", what);
    for (auto it = mQueue.begin(); it != mQueue.end();) {
        auto target = (*it)->mTarget;
        if ((*it)->what == what && (target && target.get() == handler.get())) {
            Log::i(TAG, "MessageQueue has been removed message. message=", **it);
            it = mQueue.erase(it);
        } else {
            ++it;
        }
    }

    // 测试日志
    //  for (const auto &message: mQueue) {
    //      std::cout << *(message.get()) << std::endl;
    //  }
}

void MessageQueue::removeAllMessages(std::shared_ptr<Handler> handler) {
    std::unique_lock<std::mutex> lock(mMutex);
    Log::i(TAG, "MessageQueue remove all messages. Queue size=", mQueue.size());
    for (auto it = mQueue.begin(); it != mQueue.end();) {
        auto target = (*it)->mTarget;
        if ((target && target.get() == handler.get())) {
            Log::i(TAG, "MessageQueue has been removed message. message=", **it);
            it = mQueue.erase(it);
        } else {
            ++it;
        }
    }
}

void MessageQueue::reset() {
    mIsQuit = false;
}

void MessageQueue::quit() {
    mIsQuit = true;
    mThreadCond.notify_all();
}

bool MessageQueue::isQuit() {
    return mIsQuit;
}
}