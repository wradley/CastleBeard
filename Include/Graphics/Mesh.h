#pragma once
#include "MeshData.h"

namespace Graphics
{
    class Mesh
    {
    public:

        Mesh(const MeshData &m);
        ~Mesh();

        void draw();

    private:

        unsigned int _vao, _vbo, _ebo, _numIndices;

    private:

        Mesh(const Mesh &) = delete;
        Mesh(Mesh &&) = delete;
    };
}