//
// Created by 江澎涌 on 2024/4/19.
//
#include "JHandlerDemo.h"

const std::string tag = "【main】";

void sendMessage(const std::shared_ptr<MyHandler> &myHandler, int count) {
    for (int i = 0; i < count; ++i) {
        if (i % 2 == 0) {
            myHandler->post([i]() {
                JHandler::Log::i(tag, "进入闭包函数，i=", i);
            });
        } else {
            auto message = myHandler->obtainMessage();
            message->what = i;
            message->arg1 = i * 100;
            message->arg2 = i * 1000;
            std::stringstream ss;
            ss << "jiang peng yong" << " " << i;
            message->data = std::make_shared<std::string>(ss.str());
            myHandler->sendMessage(std::move(message));
            JHandler::Log::i(tag, "创建了 Message ，并将其发送。");
        }
    }
}

// 测试发送 message ，携带 what、arg1、arg2、data
void sendWhatMessageDemo() {
    JHandler::Log::i(tag, "====================== jhandler 开始运行 —— 运行 what message ======================");

    // 创建携带线程的 HandlerThread
    auto handlerThread = JHandler::HandlerThread();
    JHandler::Log::i(tag, "创建了 HandlerThread.");

    // 创建 MyHandler 处理器
    auto myHandler = std::make_shared<MyHandler>("MyHandler", handlerThread.getLooper());
    JHandler::Log::i(tag, "创建了 MyHandler.");

    // 启动前发送信息
    sendMessage(myHandler, 5);

    // 启动线程
    handlerThread.start();

    // 启动后发送信息
//    sendMessageToFilter(myHandler, 5);

    // 退出线程
    handlerThread.quit();
    // 等待线程运行完
    handlerThread.join();

    JHandler::Log::i(tag, "====================== jhandler 正常结束 —— 运行 what message ======================");
}

// 测试移除 message
void removeMessageDemo() {
    JHandler::Log::i(tag, "====================== jhandler 开始运行 —— 运行移除 message ======================");

    // 创建携带线程的 HandlerThread
    auto handlerThread = JHandler::HandlerThread();
    JHandler::Log::i(tag, "创建了 HandlerThread.");

    // 创建 MyHandler 处理器
    auto myHandler = std::make_shared<MyHandler>("MyHandler", handlerThread.getLooper());
    JHandler::Log::i(tag, "创建了 MyHandler.");

    // 启动线程
    handlerThread.start();

    // 启动后发送信息
    sendMessage(myHandler, 8);

//    myHandler->removeAllMessages();
    myHandler->removeMessage(5);

    // 退出线程
    handlerThread.quit();
    // 等待线程运行完
    handlerThread.join();

    JHandler::Log::i(tag, "====================== jhandler 正常结束 —— 运行移除 message ======================");
}

// 测试同时多个线程运行
void multiHandlerDemo() {
    JHandler::Log::i(tag,
                     "====================== jhandler 开始运行 —— 多 Handler 使用同一 Thread ======================");

    // 创建携带线程的 HandlerThread
    auto handlerThread = JHandler::HandlerThread();
    // 创建 MyHandler 处理器
    auto myHandler1 = std::make_shared<MyHandler>("MyHandler1", handlerThread.getLooper());
    auto myHandler2 = std::make_shared<MyHandler>("MyHandler2", handlerThread.getLooper());

    // 启动线程
    handlerThread.start();

    // 启动后发送信息
    sendMessage(myHandler1, 8);
    sendMessage(myHandler2, 5);

    // 退出线程
    handlerThread.quit();
    // 等待线程运行完
    handlerThread.join();

    JHandler::Log::i(tag,
                     "====================== jhandler 正常结束 —— 多 Handler 使用同一 Thread ======================");
}

void multiThreadDemo() {
    JHandler::Log::i(tag, "====================== jhandler 开始运行 —— 多 Thread ======================");

    // 创建携带线程的 HandlerThread
    auto handlerThread1 = JHandler::HandlerThread();

    auto handlerThread2 = JHandler::HandlerThread();

    // 创建 MyHandler 处理器
    auto myHandler1 = std::make_shared<MyHandler>("MyHandler1", handlerThread1.getLooper());
    auto myHandler2 = std::make_shared<MyHandler>("MyHandler2", handlerThread2.getLooper());

    // 启动后发送信息
    sendMessage(myHandler1, 8);
    sendMessage(myHandler2, 5);

    // 启动线程
    handlerThread2.start();
    // 启动线程
    handlerThread1.start();

    // 退出线程
    handlerThread1.quit();
    handlerThread2.quit();
    // 等待线程运行完
    handlerThread1.join();
    handlerThread2.join();

    JHandler::Log::i(tag, "====================== jhandler 正常结束 —— 多 Thread ======================");
}