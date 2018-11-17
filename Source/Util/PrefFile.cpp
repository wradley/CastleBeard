#include <fstream>
#include <vector>
#include "PrefFile.h"
#include "StringParser.h"
#include "Debug.h"

namespace Util
{
    PrefFile::PrefFile()
    {
    }


    PrefFile::~PrefFile()
    {
    }


    bool PrefFile::loadFile(const std::string & filepath)
    {
        // load file contents into string
        std::ifstream file(filepath);
        std::string contents;

        if (file)
        {
            _filepath = filepath;
            file.seekg(0, std::ios::end);
            contents.reserve(file.tellg());
            file.seekg(0, std::ios::beg);
            contents.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        }
        else {
            DEBUG_LOG("Could not open pref-file: " + filepath);
            return false;
        }

        // gather data
        std::vector<std::string> lines = StringParser::Split(contents, '\n');
        for (std::string &line : lines)
        {
            // check if this is a comment
            line = StringParser::Trim(line);
            if (line[0] == '#') continue;

            // if not a comment
            std::vector<std::string> keyAndVal = StringParser::Split(line, '=');
            if (keyAndVal.size() != 2)
            {
                DEBUG_LOG("Pref-file [" + filepath + "] not formatted properly: " + line);
                continue;
            }

            std::string key = StringParser::Trim(keyAndVal[0]);
            std::string val = StringParser::Trim(keyAndVal[1]);

            // if this is a string
            if (val.find('"') != std::string::npos)
            {
                std::vector<std::string> str = StringParser::Split(val, '"');
                if (str.size() != 1)
                {
                    DEBUG_LOG("Pref-file [" + filepath + "] not formatted properly: " + line);
                    continue;
                }
                _strings[key] = str[0];
            }

            else if (val.find('.') != std::string::npos) _floats[key] = (val.size() > 1) ? std::stof(val) : 0.0f;

            else _ints[key] = (val.size() > 0) ? std::stoi(val) : 0;
        }

        return true;
    }


    int PrefFile::getInt(const std::string & key) const
    {
        if (_ints.find(key) == _ints.end()) return 0;
        return _ints.at(key);
    }


    float PrefFile::getFloat(const std::string & key) const
    {
        if (_floats.find(key) == _floats.end()) return 0.0f;
        return _floats.at(key);
    }


    std::string PrefFile::getString(const std::string & key) const
    {
        if (_strings.find(key) == _strings.end()) return std::string();
        return _strings.at(key);
    }
}