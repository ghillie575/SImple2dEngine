#include <logger.hpp>
#include <iostream>
#include <chrono>
#include <iomanip>

namespace engine
{
    void log(const std::string &message)
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::cout << "[" << std::put_time(std::localtime(&time), "%H:%M:%S") << "] [LOG] " << message << std::endl;
    }
    void warn(const std::string &message)
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::cerr << "[" << std::put_time(std::localtime(&time), "%H:%M:%S") << "] [WARN] " << message << std::endl;
    }
    void error(const std::string &message)
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::cerr << "[" << std::put_time(std::localtime(&time), "%H:%M:%S") << "] [ERROR] " << message << std::endl;
    }
} // namespace engine