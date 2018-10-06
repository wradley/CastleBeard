#include "../../Include/Graphics/Model.h"


Graphics::Model::Model(MeshDataPointers m)
{
    unsigned int numMeshDatas = m.getNumMeshDatas();
    for (unsigned int i = 0; i < numMeshDatas; ++i) {
        _meshes.push_back(new Mesh(*m.getMeshData(i)));
    }
}


Graphics::Model::~Model()
{
    for (auto *mesh : _meshes) {
        delete mesh;
    }
}


void Graphics::Model::draw()
{
    for (auto *mesh : _meshes) {
        mesh->draw();
    }
}