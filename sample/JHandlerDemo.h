//
// Created by 江澎涌 on 2024/4/25.
//

#ifndef EGLBOX_CPP_JHANDLERDEMO_H
#define EGLBOX_CPP_JHANDLERDEMO_H

#include <iostream>
#include <memory>
#include <sstream>
#include "HandlerThread.h"
#include "MyHandler.h"
#include "Message.h"

void sendMessage(const std::shared_ptr<MyHandler> &myHandler, int count);

void sendWhatMessageDemo();

void removeMessageDemo();

void multiHandlerDemo();

void multiThreadDemo();

#endif //EGLBOX_CPP_JHANDLERDEMO_H
