#include <fbxsdk.h>
#include <iostream>
#include <tuple>
#include <algorithm>
#include "../../../Include/Graphics/Environment/Importer.h"


static bool GenerateVertexFrom_TRIANGULATED_FbxMesh(
    const FbxMesh *mesh,
    const FbxGeometryElementNormal *normals,
    int index,
    Graphics::Vertex &retVertex
){
    if (normals->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
        // TODO
        std::cout << "Do not support control point mapping yet: " << std::endl;
        return false;
    }

    else if (normals->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
        int cpIndex = index;
        
        int normIndex = 0;
        if (normals->GetReferenceMode() == FbxGeometryElement::eDirect)
            normIndex = cpIndex;
        else if (normals->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
            normIndex = normals->GetIndexArray().GetAt(cpIndex);
        else {
            std::cout << "Unsupported fbx reference mode: GetMeshDataFrom_TRIANGULATED_FbxMesh_BYCONTROLPOINT" << std::endl;
            return false;
        }

        retVertex.position.x = (float)mesh->GetControlPointAt(mesh->GetPolygonVertex(cpIndex / 3, cpIndex % 3)).mData[0];
        retVertex.position.y = (float)mesh->GetControlPointAt(mesh->GetPolygonVertex(cpIndex / 3, cpIndex % 3)).mData[1];
        retVertex.position.z = (float)mesh->GetControlPointAt(mesh->GetPolygonVertex(cpIndex / 3, cpIndex % 3)).mData[2];
        retVertex.normal.x = (float)normals->GetDirectArray().GetAt(normIndex).mData[0];
        retVertex.normal.y = (float)normals->GetDirectArray().GetAt(normIndex).mData[1];
        retVertex.normal.z = (float)normals->GetDirectArray().GetAt(normIndex).mData[2];
    }

    return true;
}


static bool GetMeshDataFromFBXMesh(
    FbxManager *manager,
    FbxMesh *mesh,
    Graphics::MeshData &retMesh,
    bool compressVertices
) {
    // make sure mesh is triangulated
    if (!mesh->IsTriangleMesh()) {
        FbxGeometryConverter converter(manager);
        mesh = (FbxMesh*)converter.Triangulate(mesh, true);
        if (!mesh) {
            std::cout << "Error triangulating mesh" << std::endl;
            return false;
        } if (mesh->GetAttributeType() != FbxNodeAttribute::EType::eMesh) {
            std::cout << "Error triangulating mesh, fbx returned non-mesh attribute" << std::endl;
            return false;
        }
    }

    FbxGeometryElementNormal *normals = mesh->GetElementNormal(0);
    if (!normals) {
        std::cout << "Fbx mesh does not contain normals: GetMeshDataFromFBXMesh" << std::endl;
        return false;
    }

    if (!compressVertices) {
        auto numFbxIndices = mesh->GetPolygonVertexCount();
        for (int i = 0; i < numFbxIndices; ++i)
        {
            Graphics::Vertex vertex;
            GenerateVertexFrom_TRIANGULATED_FbxMesh(mesh, normals, i, vertex);
            retMesh.vertices.push_back(vertex);
            retMesh.indices.push_back(i);
        }
    }

    // compress vertices (actually the use indices)
    else {
        // generate new indices for combined vertex structure
        std::vector<std::tuple<unsigned int, Graphics::Vertex, unsigned int>> indexVertPairs; // vert index, vert, new index

        // push everything in list -- O(n)
        auto numFbxIndices = mesh->GetPolygonVertexCount();
        for (int i = 0; i < numFbxIndices; ++i)
        {
            Graphics::Vertex vertex;
            GenerateVertexFrom_TRIANGULATED_FbxMesh(mesh, normals, i, vertex);
            indexVertPairs.push_back(std::make_tuple(i, vertex, 0));
        }

        // sort by vertex -- O(n log n)
        std::sort(indexVertPairs.begin(), indexVertPairs.end(), [](
            const std::tuple<unsigned int, Graphics::Vertex, unsigned int> &a,
            const std::tuple<unsigned int, Graphics::Vertex, unsigned int> &b
            ) {
            if (std::get<1>(a).position.x != std::get<1>(b).position.x) return std::get<1>(a).position.x < std::get<1>(b).position.x;
            if (std::get<1>(a).position.y != std::get<1>(b).position.y) return std::get<1>(a).position.y < std::get<1>(b).position.y;
            if (std::get<1>(a).position.z != std::get<1>(b).position.z) return std::get<1>(a).position.z < std::get<1>(b).position.z;
            if (std::get<1>(a).normal.x != std::get<1>(b).normal.x) return std::get<1>(a).normal.x < std::get<1>(b).normal.x;
            if (std::get<1>(a).normal.y != std::get<1>(b).normal.y) return std::get<1>(a).normal.y < std::get<1>(b).normal.y;
            if (std::get<1>(a).normal.z != std::get<1>(b).normal.z) return std::get<1>(a).normal.z < std::get<1>(b).normal.z;
            return std::get<0>(a) < std::get<0>(b);
        });

        // make new indices -- O(n)
        unsigned int currIndex = 0;
        for (int i = 0; i < indexVertPairs.size(); ++i) {
            if (i == 0) {
                std::get<2>(indexVertPairs[i]) = currIndex;
            }

            else {
                // if this is the same as the last vertex make the new index the same as the last
                if (std::get<1>(indexVertPairs[i - 1]) == std::get<1>(indexVertPairs[i]))
                    std::get<2>(indexVertPairs[i]) = currIndex;
                else
                    std::get<2>(indexVertPairs[i]) = ++currIndex;
            }
        }

        // sort back to orig. order by old index -- O(n log n)
        std::sort(indexVertPairs.begin(), indexVertPairs.end(), [](
            const std::tuple<unsigned int, Graphics::Vertex, unsigned int> &a,
            const std::tuple<unsigned int, Graphics::Vertex, unsigned int> &b
            ) {
            return (std::get<0>(a) < std::get<0>(b));
        });

        // copy new indices into retmesh -- O(n)
        for (int i = 0; i < indexVertPairs.size(); ++i)
            retMesh.indices.push_back(std::get<2>(indexVertPairs[i]));

        // copy verts into retmesh (removing duplicates) -- O(n)
        retMesh.vertices.resize(currIndex + 1);
        for (int i = 0; i < indexVertPairs.size(); ++i) {
            retMesh.vertices[std::get<2>(indexVertPairs[i])] = std::get<1>(indexVertPairs[i]);
        }
    }

    // total -- O(n) + O(n log n) + O(n) + O(n log n) + O(n) + O(n) = O(n) + O(n log n) = O(n log n)
    // room for improvement but better than previous O(n*n)
    // tends to be about 3x slower than not compressing the vertices

    return true;
}


static bool GetMeshDataFromFBXMesh_UNOPT(
    FbxManager *manager,
    FbxMesh *mesh,
    Graphics::MeshData &retMesh
){
    // make sure mesh is triangulated
    if (!mesh->IsTriangleMesh()) {
        FbxGeometryConverter converter(manager);
        mesh = (FbxMesh*)converter.Triangulate(mesh, true);
        if (!mesh) {
            std::cout << "Error triangulating mesh" << std::endl;
            return false;
        } if (mesh->GetAttributeType() != FbxNodeAttribute::EType::eMesh) {
            std::cout << "Error triangulating mesh, fbx returned non-mesh attribute" << std::endl;
            return false;
        }
    }

    FbxGeometryElementNormal *normals = mesh->GetElementNormal(0);
    if (!normals) {
        std::cout << "Fbx mesh does not contain normals: GetMeshDataFromFBXMesh" << std::endl;
        return false;
    }

    // generate new indices for combined vertex structure
    auto numFbxIndices = mesh->GetPolygonVertexCount();
    for (int i = 0; i < numFbxIndices; ++i) 
    {
        unsigned int newIndex = 0;
        Graphics::Vertex vertex;

        GenerateVertexFrom_TRIANGULATED_FbxMesh(mesh, normals, i, vertex);

        // check to see if this vertex has already been generated
        bool found = false;
        for (auto j = 0; j < retMesh.vertices.size(); ++j) {
            if (retMesh.vertices[j] == vertex) {
                newIndex = (unsigned int)j;
                found = true;
                break;
            }
        }

        // if there isn't one already, create a new vertex entry
        if (!found) {
            newIndex = (unsigned int) retMesh.vertices.size();
            retMesh.vertices.push_back(vertex);
        }

        retMesh.indices.push_back(newIndex);
    }

    return true;
}


static bool TraverseFBXTree(
    FbxManager *manager, 
    FbxNode *node, 
    Graphics::MeshDataPointers &retMeshDatas,
    bool compressVertices
){
    bool retStatus = true;

    for (int i = 0; i < node->GetNodeAttributeCount(); ++i) {
        if (node->GetNodeAttributeByIndex(i)->GetAttributeType() == FbxNodeAttribute::EType::eMesh) {
            Graphics::MeshData *mesh = new Graphics::MeshData;
            retStatus = GetMeshDataFromFBXMesh(manager, (FbxMesh*)node->GetNodeAttributeByIndex(i), *mesh, compressVertices);
            retMeshDatas.addMeshData(std::shared_ptr<Graphics::MeshData>(mesh));
        }
    }

    for (int i = 0; i < node->GetChildCount(); ++i) {
        retStatus = TraverseFBXTree(manager, node->GetChild(i), retMeshDatas, compressVertices);
    }

    return retStatus;
}


static bool LoadFBXFile(
    const std::string &filepath, 
    Graphics::MeshDataPointers &retMeshDatas,
    bool compressVertices
){
    FbxManager *manager = FbxManager::Create();
    FbxIOSettings *ios = FbxIOSettings::Create(manager, IOSROOT);
    manager->SetIOSettings(ios);

    FbxImporter *importer = FbxImporter::Create(manager, "");
    if (!importer->Initialize(filepath.c_str(), -1, manager->GetIOSettings())) {
        std::cout << "Could not initialize FBX Importer: " << importer->GetStatus().GetErrorString() << std::endl;
        return false;
    }

    FbxScene *scene = FbxScene::Create(manager, "");
    importer->Import(scene);
    importer->Destroy();

    FbxNode *root = scene->GetRootNode();
    bool result = TraverseFBXTree(manager, root, retMeshDatas, compressVertices);

    manager->Destroy();
    return result;
}


Graphics::Importer::Importer()
{}


Graphics::Importer::~Importer()
{}


bool Graphics::Importer::loadFile(const std::string & fp, bool compressVertices)
{
    // remove old pointers
    _meshDataPointers = MeshDataPointers();
    return LoadFBXFile(fp, _meshDataPointers, compressVertices);
}


Graphics::MeshDataPointers Graphics::Importer::getMeshDataPointers()
{
    return _meshDataPointers;
}
