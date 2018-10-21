#pragma once
#include <string>
#include <iostream>
#include <mutex>

#define DEBUG_LOG(msg) Debug::Log(msg, __FILE__, __LINE__)

namespace Debug
{
    inline void Log(const std::string &message, const char *file, int line) {
        static std::mutex mtx;
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "[" << file << ", " << line << "] " << message << std::endl;
    }
}