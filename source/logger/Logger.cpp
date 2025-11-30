#include <iostream>
#include "logger.h"

namespace logger
{
    void Init()
    {
        std::ios::sync_with_stdio(false);
    }

    void Info(const std::string& msg)
    {
        std::cout << "[INFO] " << msg << std::endl;
    }

    void Error(const std::string& msg)
    {
        std::cerr << "[ERROR] " << msg << std::endl;
    }
}