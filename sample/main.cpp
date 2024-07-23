#include <iostream>
#include <string>
#include "Log.h"
#include "simpleUse/SimpleUse.h"
#include "diyThread/DiyThread.h"

using namespace std;
using namespace jhandler;

static string TAG = "JHandler-Demo";

int main() {
    jhandler::Log::isNeedShowLog = true;

    simpleUse();

    diyThread();

    return 0;
}

