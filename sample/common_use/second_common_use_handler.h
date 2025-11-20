//
// Created by 江澎涌 on 2025/11/18.
//

#ifndef JHANDLER_SAMPLE_SECOND_COMMON_USE_HANDLER_H
#define JHANDLER_SAMPLE_SECOND_COMMON_USE_HANDLER_H

#include "handler.h"

class SecondCommonUseHandler : public jhandler::Handler {
private:
    static std::string TAG;

public:
    explicit SecondCommonUseHandler(std::shared_ptr<jhandler::Looper> looper);

    void handleMessage(const std::unique_ptr<jhandler::Message> &message) override;
};



#endif //JHANDLER_SAMPLE_SECOND_COMMON_USE_HANDLER_H
