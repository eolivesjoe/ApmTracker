#pragma once

#include <string>

namespace logger
{
    void Init();

    void Info(const std::string& msg);

    void Error(const std::string& msg);
} // namespace logger