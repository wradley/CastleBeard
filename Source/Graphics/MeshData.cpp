#include "../../Include/Graphics/MeshData.h"


Graphics::MeshData::MeshData()
{}


Graphics::MeshData::MeshData(const MeshData & md) :
    vertices(md.vertices), indices(md.indices)
{}


Graphics::MeshData & Graphics::MeshData::operator=(const MeshData & md)
{
    vertices = md.vertices;
    indices = md.indices;
    return *this;
}


Graphics::MeshData::~MeshData() {}
