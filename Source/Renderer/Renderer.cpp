#include <glad/glad.h>
#include "Renderer.h"
#include "Scene.h"
#include "Camera.h"
#include "Importer.h"
#include "CBMath.h"
#include "Debug.h"
#include "../../Include/Core/TextFile.h"
#include "../../Include/Core/PrefFile.h"


namespace Renderer 
{

    Renderer::Renderer() : _rootScene(new Scene(this, nullptr)), _camera(nullptr)
    {}


    Renderer::~Renderer()
    {
        _rootScene->destroy();
        delete _rootScene;
    }


    Scene* Renderer::getRootScene()
    {
        return _rootScene;
    }


    void Renderer::setCamera(Camera *c)
    {
        _camera = c;
    }


    void Renderer::setRenderTarget(RenderTarget *rt)
    {}


    void Renderer::draw()
    {
        if (!_camera) {
            DEBUG_LOG("No set camera");
            return;
        }

        // clear screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // calculate proj & view
        Math::Mat4 proj(_camera->calculatePerspectiveMatrix());
        Math::Mat4 view(_camera->calculateViewMatrix());

        // draw everything
        _rootScene->draw(proj, view);

        // delete any old nodes
        for (RendererNode *n : _deleteList) {
            delete n;
        }
        _deleteList.clear();
    }


    void Renderer::setViewport_TEMP_(int width, int height)
    {
        glViewport(0, 0, width, height);
    }


    void Renderer::queueForDeletion(RendererNode *r)
    {
        _deleteList.push_back(r);
    }


    std::shared_ptr<Shader> Renderer::loadShader(const std::string & vFP, const std::string & gFP, const std::string & fFP)
    {
        std::string ID(vFP + gFP + fFP);

        // load shader if not in cache
        if (_shaders.find(ID) == _shaders.end()) {
            std::string vertexCode = Core::LoadTextFile(vFP);
            std::string geomCode = gFP.size() ? Core::LoadTextFile(gFP) : "";
            std::string fragCode = Core::LoadTextFile(fFP);
            const char *vCode = vertexCode.c_str();
            const char *gCode = geomCode.size() ? geomCode.c_str() : nullptr;
            const char *fCode = fragCode.c_str();
            Shader *shader = new Shader;
            shader->compileFiles(vCode, gCode, fCode);
            _shaders[ID] = std::shared_ptr<Shader>(shader);
        }

        return _shaders[ID];
    }


    std::shared_ptr<Material> Renderer::loadMaterial(const std::string & fp)
    {
        if (_materials.find(fp) == _materials.end()) {
            Core::PrefFile materialFile;
            materialFile.loadFile(fp);
            Material *material = new Material;
            // TODO load textures & colors
            _materials[fp] = std::shared_ptr<Material>(material);
        }

        return _materials[fp];
    }


    std::shared_ptr<std::vector<Mesh*>> Renderer::loadMeshes(const std::string & fp)
    {
        if (_meshes.find(fp) == _meshes.end()) {
            Importer importer;
            importer.loadFile(fp);
            std::vector<Mesh*> *meshes = new std::vector<Mesh*>;
            auto meshdataPtrs = importer.getMeshDataPointers();
            unsigned int numMeshDatas = meshdataPtrs.getNumMeshDatas();
            for (unsigned int i = 0; i < numMeshDatas; ++i) {
                meshes->push_back(new Mesh(*meshdataPtrs.getMeshData(i)));
            }
            _meshes[fp] = std::shared_ptr<std::vector<Mesh*>>(meshes);
        }

        return _meshes[fp];
    }
}