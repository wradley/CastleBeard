#include "../../Include/Graphics/Model.h"


Graphics::Model::Model(std::vector<MeshData*> meshDatas)
{
    for (MeshData *md : meshDatas) {
        _meshes.push_back(new Mesh(*md));
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