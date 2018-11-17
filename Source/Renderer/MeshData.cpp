#include "MeshData.h"

namespace Renderer
{

    MeshData::MeshData()
    {}


    MeshData::MeshData(const MeshData & md) :
        vertices(md.vertices), indices(md.indices)
    {}


    MeshData & MeshData::operator=(const MeshData & md)
    {
        vertices = md.vertices;
        indices = md.indices;
        return *this;
    }


    MeshData::~MeshData() {}


    MeshDataPointers::MeshDataPointers()
    {}


    MeshDataPointers::MeshDataPointers(const MeshDataPointers & m) :
        _datas(m._datas)
    {}


    MeshDataPointers & MeshDataPointers::operator=(const MeshDataPointers & m)
    {
        _datas = m._datas;
        return *this;
    }


    MeshDataPointers::MeshDataPointers(MeshDataPointers && m) :
        _datas(m._datas)
    {
        m._datas.clear();
    }


    MeshDataPointers::~MeshDataPointers()
    {
        _datas.clear();
    }


    void MeshDataPointers::addMeshData(std::shared_ptr<MeshData> md)
    {
        _datas.push_back(md);
    }


    unsigned int MeshDataPointers::getNumMeshDatas() const
    {
        return _datas.size();
    }


    std::shared_ptr<MeshData> MeshDataPointers::getMeshData(unsigned int i)
    {
        return _datas[i];
    }
}