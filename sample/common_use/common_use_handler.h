//
// Created by 江澎涌 on 2025/11/17.
//

#ifndef JHANDLER_SAMPLE_COMMONUSEHANDLER_H
#define JHANDLER_SAMPLE_COMMONUSEHANDLER_H

#include "handler.h"

class CommonUseHandler : public jhandler::Handler {
private:
    static std::string TAG;

public:
    explicit CommonUseHandler(std::shared_ptr<jhandler::Looper> looper);

    void handleMessage(const std::unique_ptr<jhandler::Message> &message) override;
};


#endif //JHANDLER_SAMPLE_COMMONUSEHANDLER_H
