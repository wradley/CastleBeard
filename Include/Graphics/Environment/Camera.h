#pragma once
#include "../../Math/Transform.h"

namespace Graphics
{
    class Camera
    {
    public:

        unsigned int node;
        float nearPlane;
        float farPlane;
        float fov;
        float aspectRatio;
        Math::Transform transform;

        Camera();
        ~Camera() {}

    };
}