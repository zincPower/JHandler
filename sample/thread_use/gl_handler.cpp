//
// Created by 江澎涌 on 2025/11/20.
//

#include "gl_handler.h"
#include "../log/log.h"

const std::string GLHandler::TAG = "GLHandler";

void GLHandler::handleMessage(const std::unique_ptr <jhandler::Message> &message) {
    switch (message->what) {
        case MSG_REQUEST_RENDER: {
            Log::i(TAG, "进行渲染 thread id=", std::this_thread::get_id());
            break;
        }
        case MSG_ADD_FILTER: {
            auto filterName = message->getData<std::string>();
            Log::i(TAG, "添加滤镜 filterName=", filterName, " thread id=", std::this_thread::get_id());
            break;
        }
        case MSG_REMOVE_FILTER: {
            auto filterName = message->getData<std::string>();
            Log::i(TAG, "移除滤镜 filterName=", filterName, " thread id=", std::this_thread::get_id());
            break;
        }
    }
}

void GLHandler::requestRender() {
    auto message = jhandler::Message::obtain();
    message->what = MSG_REQUEST_RENDER;
    sendMessage(std::move(message));
}

void GLHandler::addFilter() {
    auto message = jhandler::Message::obtain();
    message->what = MSG_ADD_FILTER;
    message->data = std::make_shared<std::string>("TriangleFilter");
    sendMessage(std::move(message));
}

void GLHandler::removeFilter() {
    auto message = jhandler::Message::obtain();
    message->what = MSG_REMOVE_FILTER;
    message->data = std::make_shared<std::string>("TriangleFilter");
    sendMessage(std::move(message));
}