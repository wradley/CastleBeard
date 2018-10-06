#pragma once
#include <string>
#include "MeshData.h"

namespace Graphics
{
    class Importer
    {
    public:

        Importer();
        Importer(const std::string &fp);
        ~Importer();

        void loadFile(const std::string &fp);
        MeshDataPointers getMeshDataPointers();

    private:

        MeshDataPointers _meshDataPointers;
    };
}