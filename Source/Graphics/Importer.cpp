#include <fbxsdk.h>
#include <iostream>
#include "../../Include/Graphics/Importer.h"


static void GetMeshFromFBX(
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
            return;
        } if (mesh->GetAttributeType() != FbxNodeAttribute::EType::eMesh) {
            std::cout << "Error triangulating mesh, fbx returned non-mesh attribute" << std::endl;
            return;
        }
    }

    auto numVerts = mesh->GetControlPointsCount();
    auto numIndices = mesh->GetPolygonVertexCount();
    auto fbxpositions = mesh->GetControlPoints();
    auto fbxindices = mesh->GetPolygonVertices();

    FbxArray<FbxVector4> fbxnormals;
    mesh->GetPolygonVertexNormals(fbxnormals);

    retMesh.vertices.resize(numVerts);
    retMesh.indices.resize(numIndices);

    for (int i = 0; i < numVerts; ++i) {
        retMesh.vertices[i].position.x = (float)fbxpositions[i][0];
        retMesh.vertices[i].position.y = (float)fbxpositions[i][1];
        retMesh.vertices[i].position.z = (float)fbxpositions[i][2];
    }

    for (int i = 0; i < numIndices; ++i) {
        retMesh.indices[i] = fbxindices[i];
    }
}


static void TraverseFBXTree(
    FbxManager *manager, 
    FbxNode *node, 
    Graphics::MeshDataPointers &retMeshDatas
){
    for (int i = 0; i < node->GetNodeAttributeCount(); ++i) {
        if (node->GetNodeAttributeByIndex(i)->GetAttributeType() == FbxNodeAttribute::EType::eMesh) {
            Graphics::MeshData *mesh = new Graphics::MeshData;
            GetMeshFromFBX(manager, (FbxMesh*)node->GetNodeAttributeByIndex(i), *mesh);
            retMeshDatas.addMeshData(std::shared_ptr<Graphics::MeshData>(mesh));
        }
    }

    for (int i = 0; i < node->GetChildCount(); ++i) {
        TraverseFBXTree(manager, node->GetChild(i), retMeshDatas);
    }
}


static void LoadFBXFile(
    const std::string &filepath, 
    Graphics::MeshDataPointers &retMeshDatas
){
    FbxManager *manager = FbxManager::Create();
    FbxIOSettings *ios = FbxIOSettings::Create(manager, IOSROOT);
    manager->SetIOSettings(ios);

    FbxImporter *importer = FbxImporter::Create(manager, "");
    if (!importer->Initialize(filepath.c_str(), -1, manager->GetIOSettings())) {
        std::cout << "Could not initialize FBX Importer: " << importer->GetStatus().GetErrorString() << std::endl;
        return;
    }

    FbxScene *scene = FbxScene::Create(manager, "");
    importer->Import(scene);
    importer->Destroy();

    FbxNode *root = scene->GetRootNode();
    TraverseFBXTree(manager, root, retMeshDatas);
}


Graphics::Importer::Importer()
{}


Graphics::Importer::Importer(const std::string & fp)
{
    loadFile(fp);
}


Graphics::Importer::~Importer()
{}


void Graphics::Importer::loadFile(const std::string & fp)
{
    _meshDataPointers = MeshDataPointers();
    LoadFBXFile(fp, _meshDataPointers);
}


Graphics::MeshDataPointers Graphics::Importer::getMeshDataPointers()
{
    return _meshDataPointers;
}
