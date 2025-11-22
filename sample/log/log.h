//
// Created by 江澎涌 on 2024/6/2.
//

#ifndef JHANDLER_CPP_LOG_H
#define JHANDLER_CPP_LOG_H

#include <iostream>
#include <sstream>
#include <string>

class Log {
public:
    template<typename T, typename... Args>
    static void i(const T &tag, const Args &...args) {
        std::ostringstream os;
        os << "Info:【" << tag << "】 ";
        i(os, args...);
    }

    template<typename T, typename... Args>
    static void e(const T &tag, const Args &...args) {
        std::ostringstream os;
        os << "Error: 【" << tag << "】 ";
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
        os << "\n";
        std::cout << os.str() << std::flush;
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
        os << "\n";
        std::cout << os.str() << std::flush;
    }
};

#endif // JHANDLER_CPP_LOG_H
