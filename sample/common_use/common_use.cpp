//
// Created by 江澎涌 on 2025/11/16.
//

#include <string>
#include <memory>
#include "HandlerThread.h"
#include "Log.h"
#include "common_use.h"
#include "common_use_handler.h"
#include "second_common_use_handler.h"
#include "message_type.h"

const std::string TAG = "CommonUse";

// 运行闭包
void runClosure() {
    auto handlerThread = jhandler::HandlerThread::create();
    handlerThread->start();

    auto looper = handlerThread->getLooper();
    auto handler = std::make_shared<CommonUseHandler>(looper);

    auto name = "江澎涌";
    handler->post([name]() {
        jhandler::Log::i(TAG, "【runClosure】运行闭包 name=", name, " Looper 线程 id=", std::this_thread::get_id());
    });

    handlerThread->quit();
}

// 添加事件消息
void addMessage() {
    auto handlerThread = jhandler::HandlerThread::create();
    handlerThread->start();

    auto looper = handlerThread->getLooper();
    auto handler = std::make_shared<CommonUseHandler>(looper);

    auto message = jhandler::Message::obtain();
    message->what = SAY_HI;
    message->data = std::make_shared<std::string>("江澎涌");
    handler->sendMessage(std::move(message));

    handlerThread->quit();
}

// 移除对应 what 的事件消息
void removeSingleMessage() {
    auto handlerThread = jhandler::HandlerThread::create();
    handlerThread->start();

    auto looper = handlerThread->getLooper();
    auto handler = std::make_shared<CommonUseHandler>(looper);

    auto message = jhandler::Message::obtain();
    message->what = SAY_HI;
    message->data = std::make_shared<std::string>("江澎涌");
    handler->removeMessage(SAY_HI);

    handlerThread->quit();
}

// 移除所有消息
void removeAllMessage() {
    auto handlerThread = jhandler::HandlerThread::create();
    handlerThread->start();

    auto looper = handlerThread->getLooper();
    auto handler = std::make_shared<CommonUseHandler>(looper);

    auto message = jhandler::Message::obtain();
    message->what = SAY_HI;
    message->data = std::make_shared<std::string>("江澎涌");
    handler->sendMessage(std::move(message));
    message = jhandler::Message::obtain();
    message->what = SAY_HI;
    message->data = std::make_shared<std::string>("jiang peng yong");
    handler->sendMessage(std::move(message));

    handler->removeAllMessages();

    handlerThread->quit();
}

void multiHandler() {
    auto handlerThread = jhandler::HandlerThread::create();
    handlerThread->start();

    auto looper = handlerThread->getLooper();
    auto handler1 = std::make_shared<CommonUseHandler>(looper);
    auto handler2 = std::make_shared<SecondCommonUseHandler>(looper);

    auto message = jhandler::Message::obtain();
    message->what = SAY_HI;
    message->data = std::make_shared<std::string>("江澎涌");
    handler1->sendMessage(std::move(message));

    message = jhandler::Message::obtain();
    message->what = SAY_HI;
    message->data = std::make_shared<std::string>("jiang peng yong");
    handler2->sendMessage(std::move(message));

    handlerThread->quit();
}

void commonUse() {
    runClosure();

    addMessage();
    removeSingleMessage();
    removeAllMessage();

    multiHandler();

    std::this_thread::sleep_for(std::chrono::seconds(2));
}