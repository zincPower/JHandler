//
// Created by 江澎涌 on 2025/11/17.
//

#include "first_common_use_handler.h"
#include "../log/log.h"
#include "message_type.h"

std::string FirstCommonUseHandler::TAG = "FirstCommonUseHandler";

FirstCommonUseHandler::FirstCommonUseHandler(std::shared_ptr<jhandler::Looper> looper) : Handler(looper) {}

void FirstCommonUseHandler::handleMessage(const std::unique_ptr<jhandler::Message> &message) {
    switch (message->what) {
        case SAY_HI: {
            auto name = message->getData<std::string>();
            auto year = message->arg1;
            auto height = message->arg2;
            Log::i(TAG, "【handleMessage】你好，", *name, "(", year, ",", height, ")", " Looper 线程 id=", std::this_thread::get_id());
            break;
        }
        case SHOW_DESCRIPTION: {
            Log::i(TAG, "【handleMessage】我是一个 C++ 事件循环机制 Looper 线程 id=", std::this_thread::get_id());
            break;
        }
        case REMOVE_MESSAGE: {
            Log::i(TAG, "【handleMessage】我是一个需要被移除的消息 Looper 线程 id=", std::this_thread::get_id());
            break;
        }
    }
}
