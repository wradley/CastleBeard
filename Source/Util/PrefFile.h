#pragma once
#include <map>
#include <string>

namespace Util
{
    class PrefFile
    {
    public:

        PrefFile();
        ~PrefFile();

        bool loadFile(const std::string &filepath);

        int getInt(const std::string &key) const;
        float getFloat(const std::string &key) const;
        std::string getString(const std::string &key) const;

    private:

        std::string _filepath;
        std::map<std::string, int> _ints;
        std::map<std::string, float> _floats;
        std::map<std::string, std::string> _strings;
    };
}