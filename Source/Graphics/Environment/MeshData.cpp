#include "../../../Include/Graphics/Environment/MeshData.h"


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


Graphics::MeshDataPointers::MeshDataPointers()
{}


Graphics::MeshDataPointers::MeshDataPointers(const MeshDataPointers & m) : 
    _datas(m._datas)
{}


Graphics::MeshDataPointers & Graphics::MeshDataPointers::operator=(const MeshDataPointers & m)
{
    _datas = m._datas;
    return *this;
}


Graphics::MeshDataPointers::MeshDataPointers(MeshDataPointers && m) : 
    _datas(m._datas)
{
    m._datas.clear();
}


Graphics::MeshDataPointers::~MeshDataPointers()
{
    _datas.clear();
}


void Graphics::MeshDataPointers::addMeshData(std::shared_ptr<MeshData> md)
{
    _datas.push_back(md);
}


unsigned int Graphics::MeshDataPointers::getNumMeshDatas() const
{
    return _datas.size();
}


std::shared_ptr<Graphics::MeshData> Graphics::MeshDataPointers::getMeshData(unsigned int i)
{
    return _datas[i];
}
