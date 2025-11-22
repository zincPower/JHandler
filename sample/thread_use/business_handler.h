//
// Created by 江澎涌 on 2025/11/22.
//

#ifndef JHANDLER_SAMPLE_BUSINESS_HANDLER_H
#define JHANDLER_SAMPLE_BUSINESS_HANDLER_H

#include <handler.h>
#include <looper.h>
#include <message.h>

class BusinessHandler : public jhandler::Handler {
private:
    static const std::string TAG;
    static constexpr int SAY_HELLO = 10000;

public:
    explicit BusinessHandler(std::shared_ptr<jhandler::Looper> looper) : jhandler::Handler(std::move(looper)) {}

    ~BusinessHandler() override = default;

    void handleMessage(const std::unique_ptr<jhandler::Message> &message) override;

    void sayHello();
};


#endif //JHANDLER_SAMPLE_BUSINESS_HANDLER_H
