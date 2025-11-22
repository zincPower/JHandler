//
// Created by 江澎涌 on 2024/6/2.
//

#include "message.h"
#include <iostream>
#include "log.h"

namespace jhandler {
const std::string Message::TAG = "Message";

std::unique_ptr<Message> Message::obtain() {
    Log::i(TAG, "Obtain a message.");
    return std::unique_ptr<Message>(new Message());
}

Message::Message() {
    Log::i(TAG, "Create message.");
}

Message::~Message() {
    Log::i(TAG, "Delete Message. message=", *this);
    reset();
}

void Message::reset() {
    what = 0;
    arg1 = 0;
    arg2 = 0;
    data = nullptr;
    mTarget = std::shared_ptr<Handler>();
    mCallback = nullptr;
}

void Message::recycle() {
    reset();
}

std::ostream &operator<<(std::ostream &os, const Message &msg) {
    os << " [Message what=" << msg.what << ", "
       << "arg1=" << msg.arg1 << ", "
       << "arg2=" << msg.arg2 << ", ";
    if (msg.data) {
        os << "data is set=" << msg.data << ", ";
    } else {
        os << "data is nullptr, ";
    }
    if (msg.mCallback) {
        os << "callback is set.";
    } else {
        os << "callback is nullptr.";
    }
    os << "] ";
    return os;
}
}