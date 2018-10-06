#pragma once
#include <vector>
#include "Mesh.h"
#include "MeshData.h"

namespace Graphics
{
    class Model
    {
    public:

        Model(MeshDataPointers m);
        ~Model();
        
        void draw();

    private:

        std::vector<Mesh*> _meshes;

    private:

        Model(const Model &) = delete;
        Model(Model &&) = delete;
    };

}