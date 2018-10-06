#pragma once
#include <vector>
#include "Vertex.h"

namespace Graphics
{
    class MeshData
    {
    public:

        MeshData();
        MeshData(const MeshData &md);
        MeshData& operator= (const MeshData &md);
        ~MeshData();

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
    };
}