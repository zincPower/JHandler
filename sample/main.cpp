#include "../jhandler/src/log.h"
#include "common_use/common_use.h"
#include "thread_use/thread_use.h"

int main() {
    jhandler::Log::isNeedShowLog = false;

    // 常规使用gein
//    commonUse();

    // 自定义线程
    threadUse();

    return 0;
}

