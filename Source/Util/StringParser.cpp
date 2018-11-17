#include "StringParser.h"

namespace Util
{
    std::vector<std::string> StringParser::Split(
        const std::string & strIn,
        const std::vector<char>& chars
    ) {
        std::vector<std::string> strs;
        std::size_t first(0), last(0);
        for (char currChar : strIn)
        {
            for (char currToken : chars)
            {
                if (currToken == currChar)
                {
                    // repeat chars
                    if (first == (last - 1)) ++first;

                    // actual break
                    else
                    {
                        strs.push_back(strIn.substr(first, last));
                        last = first;
                    }
                }
            }
            ++last;
        }
        return strs;
    }


    std::vector<std::string> StringParser::Split(const std::string & s, char c)
    {
        std::vector<std::string> strs;
        std::size_t first(0), last(0);

        for (; last <= s.size(); ++last)
        {
            if (s[first] == c)
            {
                ++first;
                if (last < first) last = first;
                continue;
            }

            if (s[last] == c)
            {
                strs.push_back(s.substr(first, (last - first)));
                first = last;
                continue;
            }

            if (last == s.size())
            {
                strs.push_back(s.substr(first, (last - first) + 1));
            }
        }

        return strs;
    }


    std::string StringParser::Trim(const std::string & s)
    {
        if (s.size() <= 0) return std::string();
        if (s.size() == 1)
        {
            if (s[0] == ' ' ||
                s[0] == '\t' ||
                s[0] == '\n' ||
                s[0] == '\v' ||
                s[0] == '\f' ||
                s[0] == '\r') return std::string();
            else return s;
        }

        std::size_t front(0), back(s.size() - 1);

        // from front
        for (; front < s.size(); ++front)
        {
            if (s[front] == ' ' ||
                s[front] == '\t' ||
                s[front] == '\n' ||
                s[front] == '\v' ||
                s[front] == '\f' ||
                s[front] == '\r') continue;
            else break;
        }

        // from back
        for (; back >= 0; --back)
        {
            if (s[back] == ' ' ||
                s[back] == '\t' ||
                s[back] == '\n' ||
                s[back] == '\v' ||
                s[back] == '\f' ||
                s[back] == '\r') continue;
            else break;
        }

        if (back < front) return std::string();
        return s.substr(front, (back - front) + 1);
    }
}