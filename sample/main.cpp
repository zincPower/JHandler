#include "log.h"
#include "common_use/common_use.h"

int main() {
    jhandler::Log::isNeedShowLog = true;

    commonUse();

    return 0;
}

