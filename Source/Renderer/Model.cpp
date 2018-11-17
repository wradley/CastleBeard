#include "Model.h"
#include "Scene.h"
#include "Renderer.h"
#include "Debug.h"
#include "Defines.h"
#include "../../Include/Core/PrefFile.h"

namespace Renderer 
{

    Model::Model(Renderer *renderer, Scene *parent) :
        RendererNode(renderer, (RendererNode*)parent),
        _shader(nullptr), _material(nullptr), _meshes(nullptr)
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }


    Model::~Model()
    {}


    void Model::destroy()
    {
        ((Scene*)_parent)->forgetChildModel(this);
        RendererNode::destroy();
    }


    void Model::loadFile(const std::string & filepath)
    {
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

        _shader = _renderer->loadShader(vShaderFP, gShaderFP, fShaderFP);
        _material = _renderer->loadMaterial(materialFP);
        _meshes = _renderer->loadMeshes(meshFP);
    }


    void Model::draw(const Math::Mat4 &proj, const Math::Mat4 &view)
    {
        _shader->bind();
        _shader->setVec3("uColor", Math::Vec3(1.0f, 0.2f, 0.2f));
        _shader->setMat4("uProj", proj);
        _shader->setMat4("uView", view);
        auto mat = this->calculateGlobalTransformMatrix();
        _shader->setMat4("uModel", mat);
        std::vector<Mesh*> &meshes = *(_meshes.get());
        for (Mesh *mesh : meshes) {
            mesh->draw();
        }
    }
}