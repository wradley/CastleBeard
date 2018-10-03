#pragma once
#include "glmIncludes.h"

namespace gp
{
    struct Transform
    {
        Transform();
        Transform(const Transform &t);
        Transform& operator= (const Transform &t);
        ~Transform();
        
        glm::mat4 genMatrix();
        
        glm::vec3 position;
        glm::quat rotation;
        glm::vec3 scale;
        
    };
}
