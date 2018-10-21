#pragma once
#include <vector>
#include <memory>
#include "Mesh.h"
#include "MeshData.h"
#include "Shader.h"
#include "Material.h"
#include "../../Math/Matrix.h"
#include "../../Math/Transform.h"

namespace Graphics
{
    class Model
    {
    public:

        std::shared_ptr<Shader> shader;
        std::shared_ptr<Material> material;
        std::shared_ptr<std::vector<Mesh*>> meshes;
        Math::Transform transform;

    public:

        Model();
        ~Model();

    private:

        Model(const Model &) = delete;
        Model(Model &&) = delete;
    };

}