//
// Created by 江澎涌 on 2025/11/20.
//

#include "gl_thread.h"
#include "../log/log.h"

const std::string GLThread::TAG = "GLThread";

bool GLThread::isRunning() {
    return mIsRunning;
}

bool GLThread::start() {
    if (mIsRunning) {
        Log::e(TAG, "GLThread is running. Can't start again.");
        return false;
    } else {
        Log::i(TAG, "GLThread start.");
        mIsRunning = true;
        mThread = std::thread(&loop, shared_from_this());
        return true;
    }
}

void GLThread::quit() {
    Log::i(TAG, "Quit GLThread. mIsRunning=", mIsRunning ? "true" : "false");
    if (mIsRunning) mLooper->quit();
    // 避免 thread 游离导致退出时崩溃
    if (mThread.joinable()) mThread.detach();
}

std::shared_ptr<jhandler::Looper> GLThread::getLooper() {
    return mLooper;
}

void GLThread::waitUntilReady() {
    std::unique_lock<std::mutex> lock(mReadyMutex);
    mReadyCond.wait(lock, [self = shared_from_this()]() { return self->mIsThreadReady.load(); });
}

void GLThread::loop(const std::shared_ptr<GLThread> &glThread) {
    Log::i(TAG, "------------------------ Enter GLThread. Start GL init logic. ------------------------");
    Log::i(TAG, "GLThread id=", std::this_thread::get_id());

    Log::i(TAG, "------------------------ 创建 EGL 相关环境 ------------------------");

    glThread->mLooper = jhandler::Looper::create();
    glThread->mHandler = std::make_shared<GLHandler>(glThread->mLooper);

    // 用于等待 EGL、Looper、Handler 的创建完成
    {
        std::unique_lock<std::mutex> lock(glThread->mReadyMutex);
        glThread->mIsThreadReady = true;
        glThread->mReadyCond.notify_all();
    }

    Log::i(TAG, "------------------------ 进入事件循环 ------------------------");
    glThread->mLooper->loop();
    Log::i(TAG, "------------------------ 退出事件循环 ------------------------");

    Log::i(TAG, "------------------------ 开始释放资源 ------------------------");


    Log::i(TAG, "------------------------ 释放 EGL ------------------------ ");

    Log::i(TAG, "------------------------ 释放 Handler ------------------------ ");
    glThread->mHandler->removeAllMessages();
    glThread->mHandler = nullptr;

    Log::i(TAG, "释放 Looper");
    glThread->mLooper = nullptr;
    quitLoop(glThread);
    Log::i(TAG, "------------------------ 退出 GLThread 线程 ------------------------");
}

void GLThread::quitLoop(const std::shared_ptr<GLThread> &glThread) {
    std::unique_lock<std::mutex> lock(glThread->mReadyMutex);
    Log::i(TAG, "Loop quit.");
    glThread->mIsRunning = false;
    glThread->mIsThreadReady = false;
    glThread->mReadyCond.notify_all();
}