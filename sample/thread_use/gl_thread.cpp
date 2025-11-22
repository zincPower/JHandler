//
// Created by 江澎涌 on 2025/11/20.
//

#include "gl_thread.h"
#include "../log/log.h"

const std::string GLThread::TAG = "GLThread";

GLThread::GLThread() {
    mLooper = jhandler::Looper::create();
    mHandler = std::make_shared<GLHandler>(mLooper);
}

GLThread::~GLThread() {
    mHandler = nullptr;
    mLooper = nullptr;
}

bool GLThread::isRunning() {
    return mIsRunning;
}

bool GLThread::start() {
    if (mIsRunning) {
        Log::e(TAG, "GLThread 已经启动，不在进行启动 thread id=", std::this_thread::get_id());
        return false;
    } else {
        Log::i(TAG, "GLThread 启动 thread id=", std::this_thread::get_id());
        mIsRunning = true;
        mThread = std::thread(&loop, shared_from_this());
        return true;
    }
}

void GLThread::quit() {
    Log::i(TAG, "退出 GLThread. mIsRunning=", mIsRunning ? "true" : "false", " thread id=", std::this_thread::get_id());
    if (mIsRunning) mLooper->quit();
    // 避免 thread 游离导致退出时崩溃
    if (mThread.joinable()) mThread.detach();
}

std::shared_ptr<jhandler::Looper> GLThread::getLooper() {
    return mLooper;
}

std::shared_ptr<GLHandler> GLThread::getHandler() {
    return mHandler;
}

void GLThread::loop(const std::shared_ptr<GLThread> &glThread) {
    Log::i(TAG, "------------------------ 进入 GLThread 启动 GL 逻辑 ------------------------ thread id=", std::this_thread::get_id());

    Log::i(TAG, "------------------------ 模拟创建 EGL 相关环境 ------------------------ thread id=", std::this_thread::get_id());
    // 睡眠了 500 毫秒，模拟创建 EGL
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    Log::i(TAG, "------------------------ 进入事件循环 ------------------------ thread id=", std::this_thread::get_id());
    glThread->mLooper->loop();
    Log::i(TAG, "------------------------ 退出事件循环 ------------------------ thread id=", std::this_thread::get_id());

    Log::i(TAG, "------------------------ 开始释放资源 ------------------------ thread id=", std::this_thread::get_id());

    Log::i(TAG, "------------------------ 释放 EGL ------------------------ thread id=", std::this_thread::get_id());

    Log::i(TAG, "------------------------ 释放 Handler ------------------------ thread id=", std::this_thread::get_id());
    glThread->mHandler->removeAllMessages();
    glThread->mHandler = nullptr;

    Log::i(TAG, "释放 Looper");
    glThread->mLooper = nullptr;
    quitLoop(glThread);
    Log::i(TAG, "------------------------ 退出 GLThread 线程 ------------------------ thread id=", std::this_thread::get_id());
}

void GLThread::quitLoop(const std::shared_ptr<GLThread> &glThread) {
    Log::i(TAG, "Loop 退出");
    glThread->mIsRunning = false;
}