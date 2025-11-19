//
// Created by 江澎涌 on 2025/11/17.
//

#include "common_use_handler.h"
#include "Log.h"
#include "message_type.h"

std::string CommonUseHandler::TAG = "CommonUseHandler";

CommonUseHandler::CommonUseHandler(std::shared_ptr<jhandler::Looper> looper) : Handler(looper) {}

void CommonUseHandler::handleMessage(const std::unique_ptr<jhandler::Message> &message) {
    switch (message->what) {
        case SAY_HI: {
            auto name = std::static_pointer_cast<std::string>(message->data);
            jhandler::Log::i(TAG, "【addMessage】处理事件消息 name=", *name, " Looper 线程 id=", std::this_thread::get_id());
            break;
        }
    }
}
