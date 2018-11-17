#include <fstream>
#include "TextFile.h"
#include "Debug.h"

namespace Util
{
    std::string LoadTextFile(const std::string &filepath)
    {
        // open and check file
        std::ifstream input(filepath, std::ios::in | std::ios::binary);
        if (!input.is_open()) {
            DEBUG_LOG("Could not open file: " + filepath);
            return std::string();
        }

        // alloc space in string
        std::string text;
        input.seekg(0, std::ios::end);
        text.resize(input.tellg());
        input.seekg(0, std::ios::beg);

        // load and return string
        input.read(&text[0], text.size());
        return text;
    }
}