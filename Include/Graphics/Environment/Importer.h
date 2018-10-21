#pragma once
#include <string>
#include "MeshData.h"

namespace Graphics
{
    class Importer
    {
    public:

        Importer();
        ~Importer();

        bool loadFile(const std::string &fp, bool compressVertices = true);
        MeshDataPointers getMeshDataPointers();

    private:

        MeshDataPointers _meshDataPointers;
    };
}