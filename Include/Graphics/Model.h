#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Mesh.h"
#include "MeshData.h"
#include "Shader.h"
#include "Material.h"
#include "RendererNode.h"

namespace Graphics
{
    class Renderer;
    class Scene;

    class Model : public RendererNode
    {
    public:

        Model(Renderer *renderer, Scene *parent);
        ~Model();

        void destroy() override;
        void loadFile(const std::string &filepath);

    private:

        friend class Scene;
        void draw(const Math::Mat4 &proj, const Math::Mat4 &view);

    private:

        std::shared_ptr<Shader> _shader;
        std::shared_ptr<Material> _material;
        std::shared_ptr<std::vector<Mesh*>> _meshes;

    };

}