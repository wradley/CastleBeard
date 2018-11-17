#pragma once
#include <vector>
#include <map>
#include <memory>
#include "Scene.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "Material.h"
#include "Mesh.h"

namespace Renderer
{
    class RenderTarget;
    class RendererNode;
    
    class Renderer
    {
    public:

        Renderer();
        ~Renderer();

        Scene* getRootScene();
        void setCamera(Camera *c);
        void setRenderTarget(RenderTarget *rt);
        void draw();

        // TODO: setup render targets
        void setViewport_TEMP_(int width, int height);

    private:

        friend class RendererNode;
        friend class Model;

        void queueForDeletion(RendererNode *r);
        std::shared_ptr<Shader> loadShader(const std::string &vFP, const std::string &gFP, const std::string &fFP);
        std::shared_ptr<Material> loadMaterial(const std::string &fp);
        std::shared_ptr<std::vector<Mesh*>> loadMeshes(const std::string &fp);

    private:

        Scene *_rootScene;
        Camera *_camera;

        std::vector<RendererNode*> _deleteList;
        std::map<std::string, std::shared_ptr<Shader>> _shaders;
        std::map<std::string, std::shared_ptr<Material>> _materials;
        std::map<std::string, std::shared_ptr<std::vector<Mesh*>>> _meshes;
    };
}
