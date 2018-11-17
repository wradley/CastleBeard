#pragma once
#include <vector>
#include <memory>
#include "Vertex.h"

namespace Renderer
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

    class MeshDataPointers
    {
    public:

        MeshDataPointers();
        MeshDataPointers(const MeshDataPointers &m);
        MeshDataPointers& operator= (const MeshDataPointers &m);
        MeshDataPointers(MeshDataPointers &&m);
        ~MeshDataPointers();

        void addMeshData(std::shared_ptr<MeshData> md);

        unsigned int getNumMeshDatas() const;
        std::shared_ptr<MeshData> getMeshData(unsigned int i);

    private:

        std::vector<std::shared_ptr<MeshData>> _datas;
    };
}