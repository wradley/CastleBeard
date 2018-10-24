#include "../../Include/Graphics/Environment.h"
#include "../../Include/Graphics/Environment/Importer.h"
#include "../../Include/Core/PrefFile.h"
#include "../../Include/Core/TextFile.h"
#include "../../Include/Math/Math.h"
#include "../../Include/Defines.h"
#include "../../Include/Debug.h"
#include <glad/glad.h>


Graphics::Environment::Environment() : _mainCamera(nullptr)
{
    // node 0 is an undefined node
    _nodes.push_back(nullptr);
    _models.push_back(nullptr);
    _cameras.push_back(nullptr);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


Graphics::Environment::~Environment() 
{
    for (auto *node : _nodes) {
        if (node) delete node;
    }

    for (auto *model : _models) {
        if (model) delete model;
    }
}


void Graphics::Environment::setViewport(unsigned int x, unsigned int y, unsigned int w, unsigned int h)
{
    glViewport(x, y, w, h);
}


void Graphics::Environment::draw()
{
    if (!_mainCamera) {
        DEBUG_LOG("No main camera specified");
        return;
    }

    if (_mainCamera->node >= _nodes.size() || _nodes[_mainCamera->node] == nullptr) {
        DEBUG_LOG("Main camera does not have a specified node");
        return;
    }

    // clear screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // calculate projection matrix
    Math::Mat4 proj(Math::Perspective(
        Math::ToRadians(_mainCamera->fov),
        _mainCamera->aspectRatio,
        _mainCamera->nearPlane, 
        _mainCamera->farPlane
    ));

    // calculate view matrix
    Math::Mat4 camTransform = _nodes[_mainCamera->node]->getWorldTransformMatrix() * Math::Mat4(_mainCamera->transform);
    Math::Vec3 camPos(camTransform * Math::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
    Math::Mat4 camRot(_nodes[_mainCamera->node]->getWorldRotation() * _mainCamera->transform.rotation);
    Math::Vec3 forward(camRot * Math::Vec4(0.0f, 0.0f, -1.0f, 1.0f));
    Math::Vec3 up(camRot * Math::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
    Math::Mat4 view(Math::LookAt(camPos, forward + camPos, up));

    // draw every node
    for (auto *node : _nodes) {
        if (node) {
            for (unsigned int mID : node->models) {

                Model *model = _models[mID];
                Shader *shader = model->shader.get();

                shader->bind();
                shader->setVec3("uColor", Math::Vec3(0.6f, 0.1f, 0.1f));
                shader->setMat4("uProj", proj);
                shader->setMat4("uView", view);
                auto uModel = node->getWorldTransformMatrix() * Math::Mat4(model->transform);
                shader->setMat4("uModel", node->getWorldTransformMatrix() * Math::Mat4(model->transform));
                for (Mesh *mesh : *model->meshes.get())
                    mesh->draw();
            }
        }
    }
}


unsigned int Graphics::Environment::createNode() 
{
    unsigned int newID = 0;
    if (_recycledNodeIDs.size()) {
        newID = _recycledNodeIDs.front();
        _recycledNodeIDs.pop_front();
        EnvironmentNode *newNode = new EnvironmentNode(newID);
        _nodes[newID] = newNode;
    } else {
        newID = (unsigned int)_nodes.size();
        EnvironmentNode *newNode = new EnvironmentNode(newID);
        _nodes.push_back(newNode);
    }
    
    return newID; 
}


void Graphics::Environment::addNodeChild(unsigned int pID, unsigned int cID)
{
    // check both nodes exist
    if (pID >= (unsigned int) _nodes.size() || _nodes[pID] == nullptr) {
        DEBUG_LOG("Parent node does not exist: " + std::to_string(pID));
        return;
    }

    if (cID >= (unsigned int) _nodes.size() || _nodes[cID] == nullptr) {
        DEBUG_LOG("Child node does not exist: " + std::to_string(cID));
        return;
    }

    // check for cycles and existence of nodes
    if (doesNodeHaveChild(_nodes[cID], _nodes[pID]) || pID == cID) {
        DEBUG_LOG("Tried to add parent as child");
        return;
    }

    _nodes[pID]->children.push_back(_nodes[cID]);
}


void Graphics::Environment::addNodeModel(unsigned int node, unsigned int model)
{
    // make sure node exists
    if (node >= (unsigned int) _nodes.size() || _nodes[node] == nullptr) {
        DEBUG_LOG("Could not find node: " + std::to_string(node));
        return;
    }

    // make sure model exists
    if (model >= (unsigned int) _models.size() || _models[model] == nullptr) {
        DEBUG_LOG("Could not find model: " + std::to_string(model));
        return;
    }

    _nodes[node]->models.push_back(model);
}


void Graphics::Environment::addNodeCamera(unsigned int node, unsigned int cID)
{
    // make sure node exists
    if (node >= (unsigned int)_nodes.size() || _nodes[node] == nullptr) {
        DEBUG_LOG("Could not find node: " + std::to_string(node));
        return;
    }

    // make sure camera exists
    if (cID >= (unsigned int)_cameras.size() || _cameras[cID] == nullptr) {
        DEBUG_LOG("Could not find camera: " + std::to_string(cID));
        return;
    }

    _nodes[node]->cameras.push_back(cID);
    _cameras[cID]->node = node;
}


void Graphics::Environment::setNodeTransform(unsigned int node, const Math::Transform &t)
{
    // make sure node exists
    if (node >= (unsigned int)_nodes.size() || _nodes[node] == nullptr) {
        DEBUG_LOG("Tried to add transform to non-existent node: " + std::to_string(node));
        return;
    }

    _nodes[node]->transform = t;
}


std::vector<unsigned int> Graphics::Environment::removeNodeAndChildren(unsigned int node)
{
    std::vector<unsigned int> retIDs;

    // make sure node exists
    if (node >= (unsigned int)_nodes.size() || _nodes[node] == nullptr) {
        DEBUG_LOG("Tried to delete non-existent node: " + std::to_string(node));
        return retIDs;
    }

    _nodes[node]->deleteChildren(retIDs);
    delete _nodes[node];

    // remove from map & recycle ids
    for (unsigned int nID : retIDs) {
        _nodes[nID] = nullptr;
        _recycledNodeIDs.push_back(nID);
    }

    return retIDs;
}


unsigned int Graphics::Environment::createModel() 
{
    unsigned int newID = 0;
    if (_recycledModelIDs.size()) {
        newID = _recycledModelIDs.front();
        _recycledModelIDs.pop_front();
        Model *model = new Model();
        _models[newID] = model;
    }
    else {
        newID = (unsigned int)_models.size();
        Model *model = new Model();
        _models.push_back(model);
    }

    return newID;
}


void Graphics::Environment::loadModel(unsigned int mID, const std::string &filepath)
{
    // make sure model exists
    if (mID >= (unsigned int)_models.size() || _models[mID] == nullptr) {
        DEBUG_LOG("Tried to load model into non-existant model");
        return;
    }

    // load model file
    Core::PrefFile modelFile;
    modelFile.loadFile(ASSETS_DIR + filepath);
    std::string meshFP = ASSETS_DIR + modelFile.getString("mesh-file");
    std::string vShaderFP = ASSETS_DIR + modelFile.getString("vertex-shader-file");
    std::string gShaderFP = "";
    if (modelFile.getString("geometry-shader-file") != "")
        gShaderFP = ASSETS_DIR + modelFile.getString("geometry-shader-file");
    std::string fShaderFP = ASSETS_DIR + modelFile.getString("fragment-shader-file");
    std::string materialFP = ASSETS_DIR + modelFile.getString("material-file");

    // load shader if not in cache
    if (_shaderCache.find(vShaderFP + gShaderFP + fShaderFP) == _shaderCache.end()) {
        std::string vertexCode = Core::LoadTextFile(vShaderFP);
        std::string geomCode = gShaderFP.size() ? Core::LoadTextFile(gShaderFP) : "";
        std::string fragCode = Core::LoadTextFile(fShaderFP);
        const char *vCode = vertexCode.c_str();
        const char *gCode = geomCode.size() ? geomCode.c_str() : nullptr;
        const char *fCode = fragCode.c_str();
        Shader *shader = new Shader;
        shader->compileFiles(vCode, gCode, fCode);
        _shaderCache[vShaderFP + gShaderFP + fShaderFP] = std::shared_ptr<Shader>(shader);
    }

    // load material if not in cache
    if (_materialCache.find(materialFP) == _materialCache.end()) {
        Core::PrefFile materialFile;
        materialFile.loadFile(materialFP);
        Material *material = new Material;
        // TODO load textures & colors
        _materialCache[materialFP] = std::shared_ptr<Material>(material);
    }

    // load mesh file if not in cache
    if (_meshCache.find(meshFP) == _meshCache.end()) {
        Importer importer;
        importer.loadFile(meshFP);
        std::vector<Mesh*> *meshes = new std::vector<Mesh*>;
        auto meshdataPtrs = importer.getMeshDataPointers();
        unsigned int numMeshDatas = meshdataPtrs.getNumMeshDatas();
        for (unsigned int i = 0; i < numMeshDatas; ++i) {
            meshes->push_back(new Mesh(*meshdataPtrs.getMeshData(i)));
        }
        _meshCache[meshFP] = std::shared_ptr<std::vector<Mesh*>>(meshes);
    }

    _models[mID]->shader = _shaderCache[vShaderFP + gShaderFP + fShaderFP];
    _models[mID]->material = _materialCache[materialFP];
    _models[mID]->meshes = _meshCache[meshFP];
}


void Graphics::Environment::setModelTransform(unsigned int model, const Math::Transform &t)
{
    // make sure model exists
    if (model >= (unsigned int)_models.size() || _models[model] == nullptr) {
        DEBUG_LOG("Tried set transform for non-existent model");
        return;
    }

    _models[model]->transform = t;
}


bool Graphics::Environment::doesNodeHaveChild(EnvironmentNode *parent, EnvironmentNode *child)
{
    for (EnvironmentNode *c : parent->children) {
        if (c == child) return true;
        doesNodeHaveChild(parent, c);
    }

    return false;
}



unsigned int Graphics::Environment::createCamera()
{
    unsigned int newID = 0;
    if (_recycledCameraIDs.size()) {
        newID = _recycledCameraIDs.front();
        _recycledCameraIDs.pop_front();
        Camera *camera = new Camera();
        _cameras[newID] = camera;
    }
    else {
        newID = (unsigned int)_models.size();
        Camera *camera = new Camera();
        _cameras.push_back(camera);
    }

    return newID;
}


void Graphics::Environment::setCameraAsMain(unsigned int camera)
{
    // make sure camera exists
    if (camera >= _cameras.size() || _cameras[camera] == nullptr) {
        DEBUG_LOG("Camera does not exist: " + std::to_string(camera));
        return;
    }

    _mainCamera = _cameras[camera];
}


void Graphics::Environment::setCameraTransform(unsigned int camera, const Math::Transform &t)
{
    // make sure camera exists
    if (camera >= _cameras.size() || _cameras[camera] == nullptr) {
        DEBUG_LOG("Camera does not exist: " + std::to_string(camera));
        return;
    }

    _cameras[camera]->transform = t;
}


void Graphics::Environment::setCameraNearPlane(unsigned int camera, float d)
{
    // make sure camera exists
    if (camera >= _cameras.size() || _cameras[camera] == nullptr) {
        DEBUG_LOG("Camera does not exist: " + std::to_string(camera));
        return;
    }

    _cameras[camera]->nearPlane = d;
}


void Graphics::Environment::setCameraFarPlane(unsigned int camera, float d)
{
    // make sure camera exists
    if (camera >= _cameras.size() || _cameras[camera] == nullptr) {
        DEBUG_LOG("Camera does not exist: " + std::to_string(camera));
        return;
    }

    _cameras[camera]->farPlane = d;
}


void Graphics::Environment::setCameraAspectRatio(unsigned int camera, float a)
{
    // make sure camera exists
    if (camera >= _cameras.size() || _cameras[camera] == nullptr) {
        DEBUG_LOG("Camera does not exist: " + std::to_string(camera));
        return;
    }

    _cameras[camera]->aspectRatio = a;
}


void Graphics::Environment::setCameraFieldOfView(unsigned int camera, float fov)
{
    // make sure camera exists
    if (camera >= _cameras.size() || _cameras[camera] == nullptr) {
        DEBUG_LOG("Camera does not exist: " + std::to_string(camera));
        return;
    }

    _cameras[camera]->fov = fov;
}