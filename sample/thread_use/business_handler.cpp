//
// Created by 江澎涌 on 2025/11/22.
//

#include "business_handler.h"
#include "../log/log.h"

const std::string BusinessHandler::TAG = "BusinessHandler";

void BusinessHandler::handleMessage(const std::unique_ptr<jhandler::Message> &message) {
    switch (message->what) {
        case SAY_HELLO: {
            Log::i(TAG, "你好 thread id=", std::this_thread::get_id());
            break;
        }
    }
}

void BusinessHandler::sayHello() {
    auto message = jhandler::Message::obtain();
    message->what = SAY_HELLO;
    sendMessage(std::move(message));
}