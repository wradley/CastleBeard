#pragma once
#include <vector>
#include <string>

namespace Util
{
    namespace StringParser
    {
        std::vector<std::string> Split(
            const std::string &s,
            const std::vector<char> &chars
        );

        std::vector<std::string> Split(
            const std::string &s,
            char c
        );

        std::string Trim(const std::string &s);
    }
}

