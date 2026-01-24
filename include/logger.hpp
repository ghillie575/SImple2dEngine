#pragma once
#include <string>
namespace engine
{
    void log(const std::string &message);
    void warn(const std::string &message);
    void error(const std::string &message);
} // namespace engine