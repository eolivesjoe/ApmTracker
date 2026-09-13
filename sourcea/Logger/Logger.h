#pragma once

#include <string>

namespace logger
{
    void init();

    void info(const std::string& msg);

    void error(const std::string& msg);
} // namespace logger