//
// Created by 江澎涌 on 2025/11/20.
//

#include "thread_use.h"
#include "gl_handler.h"
#include "gl_thread.h"

void threadUse() {
    auto glThread = std::make_shared<GLThread>();
    if (glThread->start()) {
        glThread->waitUntilReady();
    }

    auto glHandler = std::make_shared<GLHandler>(glThread->getLooper());
    glHandler->addFilter();
    glHandler->requestRender();
    glHandler->removeFilter();

    glThread->quit();

    std::this_thread::sleep_for(std::chrono::seconds(1));
}