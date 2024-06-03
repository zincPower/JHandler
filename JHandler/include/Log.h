//
// Created by 江澎涌 on 2024/6/2.
//

#ifndef JHANDLER_CPP_LOG_H
#define JHANDLER_CPP_LOG_H

#include <iostream>
#include <sstream>
#include <string>

namespace jhandler {
class Log {
public:
    static bool isNeedShowLog;

    template<typename T, typename... Args>
    static void i(const T &tag, const Args &...args) {
        if(!isNeedShowLog) return;
        std::ostringstream os;
        os << "【" << tag << "】 ";
        i(os, args...);
    }

    template<typename T, typename... Args>
    static void e(const T &tag, const Args &...args) {
        if(!isNeedShowLog) return;
        std::ostringstream os;
        os << "【" << tag << "】 ";
        e(os, args...);
    }

private:
    template<typename T, typename... Args>
    static void i(std::ostringstream &os, const T &msg, const Args &...args) {
        os << msg;
        i(os, args...);
    }

    template<typename T>
    static void i(std::ostringstream &os, const T &msg) {
        os << msg;
        i(os);
    }

    static void i(std::ostringstream &os) {
        std::string msg = os.str();
        std::cout << "Info: " << os.str() << std::endl;
    }

    template<typename T, typename... Args>
    static void e(std::ostringstream &os, const T &msg, const Args &...args) {
        os << msg;
        e(os, args...);
    }

    template<typename T>
    static void e(std::ostringstream &os, const T &msg) {
        os << msg;
        e(os);
    }

    static void e(std::ostringstream &os) {
        std::string msg = os.str();
        std::cout << "Error: " << os.str() << std::endl;
    }
};
}

#endif // JHANDLER_CPP_LOG_H
