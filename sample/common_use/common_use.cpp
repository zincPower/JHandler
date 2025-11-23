//
// Created by 江澎涌 on 2025/11/16.
//

#include "common_use.h"
#include <string>
#include <memory>
#include <handler_thread.h>
#include "../log/log.h"
#include "first_common_use_handler.h"
#include "second_common_use_handler.h"
#include "message_type.h"

const std::string TAG = "CommonUse";

// 运行闭包
void runClosure() {
    auto handlerThread = jhandler::HandlerThread::create();
    handlerThread->start();

    auto looper = handlerThread->getLooper();
    auto handler = std::make_shared<FirstCommonUseHandler>(looper);

    auto name = "江澎涌";
    handler->post([name]() {
        Log::i(TAG, "【runClosure】运行闭包 name=", name, " Looper 线程 id=", std::this_thread::get_id());
    });

    handlerThread->quit();
}

// 添加事件消息
void addMessage() {
    auto handlerThread = jhandler::HandlerThread::create();
    handlerThread->start();

    auto looper = handlerThread->getLooper();
    auto handler = std::make_shared<FirstCommonUseHandler>(looper);

    auto message = jhandler::Message::obtain();
    message->what = SAY_HI;
    message->data = std::make_shared<std::string>("江澎涌");
    message->arg1 = 1994;
    message->arg2 = 170;
    handler->sendMessage(std::move(message));

    message = jhandler::Message::obtain();
    message->what = SHOW_DESCRIPTION;
    handler->sendMessage(std::move(message));

    handlerThread->quit();
}

// 移除对应 what 的事件消息
void removeSingleMessage() {
    auto handlerThread = jhandler::HandlerThread::create();
    handlerThread->start();

    auto looper = handlerThread->getLooper();
    auto handler = std::make_shared<FirstCommonUseHandler>(looper);

    auto message = jhandler::Message::obtain();
    message->what = REMOVE_MESSAGE;
    handler->removeMessage(REMOVE_MESSAGE);

    handlerThread->quit();
}

// 移除所有消息
void removeAllMessage() {
    auto handlerThread = jhandler::HandlerThread::create();
    handlerThread->start();

    auto looper = handlerThread->getLooper();
    auto handler = std::make_shared<FirstCommonUseHandler>(looper);

    auto name = "江澎涌";
    handler->post([name]() {
        Log::i(TAG, "【runClosure】闭包移除 name=", name, " Looper 线程 id=", std::this_thread::get_id());
    });
    auto message = jhandler::Message::obtain();
    message->what = SAY_HI;
    message->data = std::make_shared<std::string>("江澎涌");
    message->arg1 = 1994;
    message->arg2 = 170;
    handler->sendMessage(std::move(message));
    message = jhandler::Message::obtain();
    message->what = SAY_HI;
    message->data = std::make_shared<std::string>("jiang peng yong");
    message->arg1 = 2025;
    message->arg2 = 100;
    handler->sendMessage(std::move(message));

    handler->removeAllMessages();

    handlerThread->quit();
}

void multiHandler() {
    auto handlerThread = jhandler::HandlerThread::create();
    handlerThread->start();

    auto looper = handlerThread->getLooper();
    auto handler1 = std::make_shared<FirstCommonUseHandler>(looper);
    auto handler2 = std::make_shared<SecondCommonUseHandler>(looper);

    auto message = jhandler::Message::obtain();
    message->what = SAY_HI;
    message->data = std::make_shared<std::string>("江澎涌");
    message->arg1 = 1994;
    message->arg2 = 170;
    handler1->sendMessage(std::move(message));

    message = jhandler::Message::obtain();
    message->what = SAY_HI;
    message->data = std::make_shared<std::string>("jiang peng yong");
    message->arg1 = 2025;
    message->arg2 = 100;
    handler2->sendMessage(std::move(message));

    // handler 之间的移除消息不相互影响
    message = jhandler::Message::obtain();
    message->what = REMOVE_MESSAGE;
    handler1->sendMessage(std::move(message));
    message = jhandler::Message::obtain();
    message->what = REMOVE_MESSAGE;
    handler2->sendMessage(std::move(message));
    // 移除 handler1 what=REMOVE_MESSAGE 的消息，handler2 的消息还是会继续运行
    handler1->removeMessage(REMOVE_MESSAGE);
    // 移除 handler1 所有的消息，handler2 的消息会继续运行
//    handler1->removeAllMessages();

    handlerThread->quit();
}

void commonUse() {
    runClosure();

    addMessage();
    removeSingleMessage();
    removeAllMessage();

    multiHandler();

    // 为了让内部执行外，才结束运行
    std::this_thread::sleep_for(std::chrono::seconds(1));
}