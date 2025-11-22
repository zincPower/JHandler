//
// Created by 江澎涌 on 2025/11/20.
//

#include "thread_use.h"
#include "gl_handler.h"
#include "gl_thread.h"
#include "business_handler.h"

void threadUse() {
    auto glThread = std::make_shared<GLThread>();
    if (glThread->start()) {
        glThread->waitUntilReady();
    }

    // 创建业务需要的 handler ，可以不耦合 gl 的相关流程
    auto businessHandler = std::make_shared<BusinessHandler>(glThread->getLooper());
    businessHandler->sayHello();

    auto glHandler = glThread->getHandler();
    glHandler->addFilter();
    glHandler->requestRender();
    glHandler->removeFilter();

    businessHandler->sayHello();

    glThread->quit();

    std::this_thread::sleep_for(std::chrono::seconds(1));
}