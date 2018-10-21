#pragma once
#include <vector>
#include <tuple>
#include "Model.h"
#include "Shader.h"
#include "../../Math/Transform.h"
#include "../../Math/Matrix.h"

namespace Graphics
{
    class EnvironmentNode
    {
    public:

        unsigned int ID;
        EnvironmentNode* parent;
        std::vector<EnvironmentNode*> children;

        Math::Transform transform;

        // components
        std::vector<unsigned int> models;
        // std::vector<unsigned int> lights;
        std::vector<unsigned int> cameras;

    public:

        EnvironmentNode(unsigned int ID);
        ~EnvironmentNode();

        Math::Mat4 getWorldTransformMatrix() const;
        Math::Quat getWorldRotation() const;
        void deleteChildren(std::vector<unsigned int> &deletedIDs);
    };
}