#pragma once

#include <vector>
#include <string>

namespace comet
{
    class StringUtils
    {
    public:
        static void split(std::vector<std::string>& result, const std::string& toSplit, const char token = ' ');
    };
    
} // namespace comet
