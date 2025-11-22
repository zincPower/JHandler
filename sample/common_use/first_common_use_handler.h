//
// Created by 江澎涌 on 2025/11/17.
//

#ifndef JHANDLER_SAMPLE_FIRST_COMMON_USE_HANDLER_H
#define JHANDLER_SAMPLE_FIRST_COMMON_USE_HANDLER_H

#include "handler.h"

class FirstCommonUseHandler : public jhandler::Handler {
private:
    static std::string TAG;

public:
    explicit FirstCommonUseHandler(std::shared_ptr<jhandler::Looper> looper);

    void handleMessage(const std::unique_ptr<jhandler::Message> &message) override;
};


#endif //JHANDLER_SAMPLE_FIRST_COMMON_USE_HANDLER_H
