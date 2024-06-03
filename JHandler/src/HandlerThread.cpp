//
// Created by 江澎涌 on 2024/6/2.
//

#include "HandlerThread.h"
#include "Log.h"

namespace jhandler {
const std::string HandlerThread::TAG = "HandlerThread";

HandlerThread::HandlerThread() : mLooper(Looper::create()) {
    Log::i(TAG, "Create handler thread.");
}

HandlerThread::~HandlerThread() {
    Log::i(TAG, "Delete handler thread.");
    mLooper = nullptr;
}

void HandlerThread::start() {
    if (mThread.joinable()) {
        Log::i(TAG, "HandlerThread has been launched. Can't launch again.");
        return;
    } else {
        Log::i(TAG, "Launch handler thread.");
    }
    mThread = std::thread(&loop, mLooper);
}

void HandlerThread::quit() {
    Log::i(TAG, "Quit handler thread.");
    mLooper->quit();
}

std::shared_ptr<Looper> HandlerThread::getLooper() {
    return mLooper;
}

void HandlerThread::loop(std::shared_ptr<Looper> looper) {
    auto threadId = std::this_thread::get_id();
    Log::i(TAG, "Loop launch. Thread id=", threadId);
    looper->loop();
    Log::i(TAG, "Loop quit. Thread id=", threadId);
}

void HandlerThread::join() {
    if (mThread.joinable()) {
        mThread.join();
    }
}

void HandlerThread::detach() {
    if (mThread.joinable()) {
        mThread.detach();
    }
}
}