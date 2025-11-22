//
// Created by 江澎涌 on 2025/11/20.
//

#ifndef JHANDLER_SAMPLE_GL_HANDLER_H
#define JHANDLER_SAMPLE_GL_HANDLER_H

#include <handler.h>
#include <looper.h>
#include <message.h>

class GLHandler : public jhandler::Handler {
private:
    static const std::string TAG;
    static constexpr int MSG_REQUEST_RENDER = 10000;
    static constexpr int MSG_ADD_FILTER = 10001;
    static constexpr int MSG_REMOVE_FILTER = 10002;

public:
    explicit GLHandler(std::shared_ptr<jhandler::Looper> looper) : jhandler::Handler(std::move(looper)) {}

    ~GLHandler() override = default;

    void handleMessage(const std::unique_ptr<jhandler::Message> &message) override;

    void requestRender();

    void addFilter();

    void removeFilter();
};


#endif //JHANDLER_SAMPLE_GL_HANDLER_H
