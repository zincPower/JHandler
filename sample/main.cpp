#include <iostream>
#include <string>
#include "Log.h"
#include "simpleUse/SimpleUse.h"

using namespace std;
using namespace jhandler;

static string TAG = "JHandler-Demo";

int main() {
    Log::i(TAG, "================================== 开始 demo ==================================", " 主线程 id=", this_thread::get_id());

    simpleUse();

    Log::i(TAG, "================================== 结束 demo =================================="," 主线程 id=", this_thread::get_id());
    return 0;
}

